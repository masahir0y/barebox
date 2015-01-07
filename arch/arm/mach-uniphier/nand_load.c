#include <debug_ll.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <asm/io.h>
#include <mach/boot-device.h>
#include <mach/early_clock.h>

#define ECC_ENABLE				0xe0
#define     ECC_ENABLE__FLAG				0x0001

#define PAGES_PER_BLOCK				0x150

#define DEVICE_MAIN_AREA_SIZE			0x170

#define DEVICE_SPARE_AREA_SIZE			0x180

#define INTR_STATUS(__bank)	(0x410 + ((__bank) * 0x50))
#define     INTR_STATUS__ECC_UNCOR_ERR			0x0001
#define     INTR_STATUS__DMA_CMD_COMP			0x0004
#define     INTR_STATUS__LOAD_COMP			0x0040

#define DMA_ENABLE				0x700
#define     DMA_ENABLE__FLAG				0x0001

#define INDEX_CTRL_REG    0x0
#define INDEX_DATA_REG    0x10

#define SPARE_ACCESS		0x41
#define MAIN_ACCESS		0x42
#define PIPELINE_ACCESS		0x2000

#define MODE_01    0x04000000
#define MODE_10    0x08000000
#define BANK(x) ((x) << 24)
#define DMA_INT				(1 << 24)
#define DMA_BURST_LEN(x)		((x) & 0xffff << 16)

#define FLASH_BANK	0
#define DENALI_NAND_DATA_BASE		0x68000000
#define DENALI_REG_BASE			0x68001000

static void index_addr(u32 address, u32 data)
{
	writel(address, DENALI_NAND_DATA_BASE + INDEX_CTRL_REG);
	writel(data, DENALI_NAND_DATA_BASE + INDEX_DATA_REG);
}

static void denali_clear_interrupts(void)
{
	/* clear all bits of intr_status. */
	writel(U32_MAX, DENALI_REG_BASE + INTR_STATUS(FLASH_BANK));
}

static void denali_enable_dma(bool en)
{
	writel(en ? DMA_ENABLE__FLAG : 0, DENALI_REG_BASE + DMA_ENABLE);
}

static void denali_enable_ecc(bool en)
{
	writel(en ? ECC_ENABLE__FLAG : 0, DENALI_REG_BASE + ECC_ENABLE);
}

static int wait_for_irq(u32 irq_mask)
{
	unsigned long wait = 1000000;
	u32 intr_status;

	do {
		early_udelay(1);
		intr_status = readl(DENALI_REG_BASE + INTR_STATUS(FLASH_BANK));

		if (intr_status & INTR_STATUS__ECC_UNCOR_ERR) {
			puts_ll("error: uncorrected ECC detected\n");
			return -EIO;
		}

		if (intr_status & irq_mask)
			break;
	} while (wait--);

	if (wait < 0) {
		puts_ll("error: timeout\n");
		return -EIO;
	}

	return 0;
}

/*
 * denali_block_isbad - Check the bad block marker
 * @page:	the page of which BBM is checked
 * @oobsize:	the size of OOB
 *
 * Return 1 is if the block at @page is bad, 0 if it is good,
 * a nagative value if an error occurs.
 */
static int denali_block_isbad(int page, unsigned long oob_size)
{
	u32 addr;
	u8 bbm;
	int i, ret;
	const int page_count = 1;

	denali_enable_ecc(false);
	denali_enable_dma(false);
	denali_clear_interrupts();

	addr = BANK(FLASH_BANK) | page;

	/* setup the acccess type */
	index_addr(MODE_10 | addr, SPARE_ACCESS);

	/* setup the pipeline command */
	index_addr(MODE_10 | addr, PIPELINE_ACCESS | page_count);

	writel(MODE_01 | addr, DENALI_NAND_DATA_BASE + INDEX_CTRL_REG);

	ret = wait_for_irq(INTR_STATUS__LOAD_COMP);
	if (ret < 0)
		return ret;

	/* read the first byte of the OOB area */
	bbm = readl(DENALI_NAND_DATA_BASE + INDEX_DATA_REG);
	/* throw away the rest of the OOB data */
	for (i = 0; i < oob_size / 4 - 1; i++)
		readl(DENALI_NAND_DATA_BASE + INDEX_DATA_REG);

	return bbm != 0xff;
}


static int denali_page_read(unsigned long dest, int page, int page_count)
{
	u32 addr;

	addr = MODE_10 | BANK(FLASH_BANK) | page;

	denali_enable_ecc(true);
	denali_enable_dma(true);
	denali_clear_interrupts();

	/* setup the acccess type */
	index_addr(addr, MAIN_ACCESS);

	/* 1. setup transfer type, interrupt when complete,
	      burst len, the number of pages */
	index_addr(addr, DMA_INT | DMA_BURST_LEN(64) | 0x2000 | page_count);

	/* 2. set memory low address bits 31:0 */
	index_addr(addr, dest);

	/* 3. set memory high address bits 64:32 */
	index_addr(addr, 0);

	return wait_for_irq(INTR_STATUS__DMA_CMD_COMP);
}

int nand_load(unsigned long dest, unsigned long size)
{
	unsigned long page_size, oob_size, block_size;
	int pages_per_block, page, pages_to_read;
	int ret = 0;

	page_size = readl(DENALI_REG_BASE + DEVICE_MAIN_AREA_SIZE);
	oob_size = readl(DENALI_REG_BASE + DEVICE_SPARE_AREA_SIZE);
	pages_per_block = readl(DENALI_REG_BASE + PAGES_PER_BLOCK);
	block_size = page_size * pages_per_block;

	page = 0;
	pages_to_read = DIV_ROUND_UP(size, page_size);

	while (pages_to_read > 0) {
		int page_count;

		ret = denali_block_isbad(page, oob_size);
		if (ret < 0)
			return ret;

		if (ret) {
			page += pages_per_block;
			continue;
		}

		page_count = min(pages_to_read, pages_per_block);

		ret = denali_page_read(dest, page, page_count);
		if (ret < 0)
			return ret;

		dest += block_size;
		page += page_count;
		pages_to_read -= page_count;
	}

	return ret;
}
