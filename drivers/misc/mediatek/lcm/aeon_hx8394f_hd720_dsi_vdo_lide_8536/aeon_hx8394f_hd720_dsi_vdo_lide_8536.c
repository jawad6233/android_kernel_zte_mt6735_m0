  /*****************************************************************************
  *  Copyright Statement:
  *  --------------------
  *  This software is protected by Copyright and the information contained
  *  herein is confidential. The software may not be copied and the information
  *  contained herein may not be used or disclosed except with the written
  *  permission of MediaTek Inc. (C) 2008
  *
  *  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
  *  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
  *  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
  *  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
  *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
  *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
  *  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
  *  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
  *  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
  *  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
  *  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
  *  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
  *
  *  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
  *  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
  *  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
  *  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
  *  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
  *
  *  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
  *  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
  *  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
  *  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
  *  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
  *
  *****************************************************************************/
  
#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"
  
#ifdef BUILD_LK
	#include <mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mt-plat/mt_gpio.h>
#endif

  // ---------------------------------------------------------------------------
  //  Local Constants
  // ---------------------------------------------------------------------------
  
  #define FRAME_WIDTH                                                                             (720)
  #define FRAME_HEIGHT                                                                            (1280)
  
  
  #define REGFLAG_DELAY             							0XFEFF
  #define REGFLAG_END_OF_TABLE      							0xFFFF   // END OF REGISTERS MARKER
  
  #define   LCM_DSI_CMD_MODE                                                      0
  
  #define LCM_ID_HX8394f                                      0x94
#define GPIO_LCD_MAKER_ID 2
#define GPIO_LCM_ID2_PIN 64
  // ---------------------------------------------------------------------------
  //  Local Variables
  // ---------------------------------------------------------------------------
  
  static LCM_UTIL_FUNCS lcm_util = {0};
  
  #define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))
  
  #define UDELAY(n) (lcm_util.udelay(n))
  #define MDELAY(n) (lcm_util.mdelay(n))
  
  // ---------------------------------------------------------------------------
  //  Local Functions
  // ---------------------------------------------------------------------------
  #define dsi_set_cmdq_V3(para_tbl,size,force_update)        lcm_util.dsi_set_cmdq_V3(para_tbl,size,force_update)
  #define dsi_set_cmdq_V2(cmd, count, ppara, force_update)        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
  #define dsi_set_cmdq(pdata, queue_size, force_update)           lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
  #define wrtie_cmd(cmd)                                                                          lcm_util.dsi_write_cmd(cmd)
  #define write_regs(addr, pdata, byte_nums)                                      lcm_util.dsi_write_regs(addr, pdata, byte_nums)
  #define read_reg                                                                                        lcm_util.dsi_read_reg()
  #define read_reg_v2(cmd, buffer, buffer_size)                           lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size) 
  
  struct LCM_setting_table
  {
      unsigned cmd;
      unsigned char count;
      unsigned char para_list[128];
  };
 /* 
  static struct LCM_setting_table lcm_initialization_setting[] = {
          
          
		{0xBC,2,{0x04,0x00}},

		{0xBF,4,{0xB9,0xFF,0x83,0x94}},

		{0xBC,2,{0x0B,0x00}},

		{0xBF,11,{0xB1,0x48,0x0C,0x6C,0x09,0x33,0x54,0x71,0x71,0x2E,0x45}},

		{0xBC,2,{0x07,0x00}},

		{0xBF,7,{0xBA,0x63,0x03,0x68,0x6B,0xB2,0xC0}},
		{0xBC,2,{0x07,0x00}},
		{0xBF,7,{0xB2,0x00,0x80,0x64,0x0E,0x0A,0x2F}},
		{0xBC,2,{0x16,0x00}},
		{0xBF,22,{0xB4,0x20,0x7C,0x20,0x7C,0x20,0x7C,0x01,0x0C,0x86,0x75,0x00,0x3F,0x20,0x7C,0x20,0x7C,0x20,0x7C,0x01,0x0C,0x86}},
		{0xBC,2,{0x22,0x00}},
		{0xBF,34,{0xD3,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x32,0x10,0x03,0x00,0x03,0x32,0x13,0xC0,0x00,0x00,0x32,0x10,0x08,0x00,0x00,0x37,0x04,0x05,0x05,0x37,0x05,0x05,0x47,0x0E,0x40}},
		{0xBC,2,{0x2D,0x00}},
		{0xBF,45,{0xD5,0x18,0x18,0x18,0x18,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x19,0x19,0x19,0x19,0x20,0x21,0x22,0x23}},
		{0xBC,2,{0x2D,0x00}},
		{0xBF,45,{0xD6,0x18,0x18,0x19,0x19,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x19,0x19,0x18,0x18,0x23,0x22,0x21,0x20,}},
		{0xBC,2,{0x3B,0x00}},
		{0xBF,59,{0xE0,0x00,0x01,0x08,0x0F,0x12,0x17,0x1A,0x18,0x35,0x44,0x56,0x56,0x60,0x73,0x7B,0x80,0x8E,0x91,0x8B,0x99,0xAB,0x56,0x54,0x58,0x5B,0x62,0x6A,0x77,0x7F,0x00,0x01,0x08,0x0F,0x12,0x17,0x1A,0x18,0x35,0x44,0x54,0x55,0x60,0x73,0x7A,0x7F,0x8D,0x90,0x8C,0x9A,0xAC,0x55,0x54,0x58,0x5C,0x36,0x6B,0x78,0x7F,}},
		{0xBC,2,{0x02,0x00}},
		{0xBF,2,{0xCC,0x03}},
		{0xBC,2,{0x03,0x00}},
		{0xBF,3,{0xC0,0x1F,0x73}},
		{0xBC,2,{0x03,0x00}},
		{0xBF,3,{0xB6,0x4E,0x4E}},
		{0xBC,2,{0x02,0x00}},
		{0xBF,2,{0xD4,0x02}},
		{0xBC,2,{0x02,0x00}},
		{0xBF,2,{0xBD,0x01}},
		
		{0xBC,2,{0x02,0x00}},
		{0xBF,2,{0xB1,0x60}},
		{0xBC,2,{0x02,0x00}},
		{0xBF,2,{0xBD,0x00}},
		{0xBC,2,{0x08,0x00}},
		{0xBF,8,{0xBF,0x40,0x81,0x50,0x00,0x1A,0xFC,0x01}},
		{0xBC,2,{0x01,0x00}},
		{0xBF,1,{0x11}},
		{0xBC,2,{0x0D,0x00}},
		{0xBF,13,{0xB2,0x00,0x80,0x64,0x0E,0x0A,0x2F,0x00,0x00,0x00,0x00,0xC0,0x18}},


       //Sleep,0xOut                            
    {0x11, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 1, {0x00}},
	{REGFLAG_DELAY, 20, {}},
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
          
      };
*/ 
 
void init_lcm_registers_lide(void)
{
unsigned int data_array[16];

data_array[0] = 0x00043902;
data_array[1] = 0x9483FFB9;
dsi_set_cmdq(data_array, 2, 1);
MDELAY(1);

data_array[0] = 0x000B3902;
data_array[1] = 0x6C0C48B1;
data_array[2] = 0x71543309;
data_array[3] = 0x00452E71;
dsi_set_cmdq(data_array, 4, 1);
MDELAY(1);

data_array[0] = 0x00073902;
data_array[1] = 0x680363BA;
data_array[2] = 0x00C0B26B;
dsi_set_cmdq(data_array, 3, 1);
MDELAY(1);

data_array[0] = 0x00073902;
data_array[1] = 0x648000B2;
data_array[2] = 0x002F0A0E;
dsi_set_cmdq(data_array, 3, 1);
MDELAY(1);

data_array[0] = 0x00163902;
data_array[1] = 0x207C20B4;
data_array[2] = 0x017C207C;
data_array[3] = 0x0075860C;
data_array[4] = 0x207C203F;
data_array[5] = 0x017C207C;
data_array[6] = 0x0000860C;
dsi_set_cmdq(data_array, 7, 1);
MDELAY(1);

data_array[0] = 0x00223902;
data_array[1] = 0x000000D3;
data_array[2] = 0x10000000;
data_array[3] = 0x03103210;
data_array[4] = 0x13320300;
data_array[5] = 0x320000C0;
data_array[6] = 0x00000810;
data_array[7] = 0x05050437;
data_array[8] = 0x47050537;
data_array[9] = 0x0000400E;
dsi_set_cmdq(data_array, 10, 1);
MDELAY(1);

data_array[0] = 0x002D3902;
data_array[1] = 0x181818D5;
data_array[2] = 0x02010018;
data_array[3] = 0x06050403;
data_array[4] = 0x18181807;
data_array[5] = 0x18181818;
data_array[6] = 0x18181818;
data_array[7] = 0x18181818;
data_array[8] = 0x18181818;
data_array[9] = 0x18181818;
data_array[10] = 0x19191918;
data_array[11] = 0x22212019;
data_array[12] = 0x00000023;
dsi_set_cmdq(data_array, 13, 1);
MDELAY(1);

data_array[0] = 0x002D3902;
data_array[1] = 0x191818D6;
data_array[2] = 0x05060719;
data_array[3] = 0x01020304;
data_array[4] = 0x18181800;
data_array[5] = 0x18181818;
data_array[6] = 0x18181818;
data_array[7] = 0x18181818;
data_array[8] = 0x18181818;
data_array[9] = 0x18181818;
data_array[10] = 0x18191918;
data_array[11] = 0x21222318;
data_array[12] = 0x00000020;
dsi_set_cmdq(data_array, 13, 1);
MDELAY(1);

data_array[0] = 0x003B3902;
data_array[1] = 0x080100E0;
data_array[2] = 0x1A17120F;
data_array[3] = 0x56443518;
data_array[4] = 0x7B736056;
data_array[5] = 0x8B918E80;
data_array[6] = 0x5456AB99;
data_array[7] = 0x6A625B58;
data_array[8] = 0x01007F77;
data_array[9] = 0x17120F08;
data_array[10] = 0x4435181A;
data_array[11] = 0x73605554;
data_array[12] = 0x908D7F7A;
data_array[13] = 0x55AC9A8C;
data_array[14] = 0x635C5854;
data_array[15] = 0x007F786B;
dsi_set_cmdq(data_array, 16, 1);
MDELAY(1);

data_array[0] = 0x00023902;
data_array[1] = 0x00000BCC;
dsi_set_cmdq(data_array, 2, 1);
MDELAY(1);

data_array[0] = 0x00033902;
data_array[1] = 0x00731FC0;
dsi_set_cmdq(data_array, 2, 1);
MDELAY(1);

data_array[0] = 0x00033902;
data_array[1] = 0x006C6CB6;
dsi_set_cmdq(data_array, 2, 1);
MDELAY(1);

data_array[0] = 0x00023902;
data_array[1] = 0x000002D4;
dsi_set_cmdq(data_array, 2, 1);
MDELAY(1);

data_array[0] = 0x00023902;
data_array[1] = 0x000001BD;
dsi_set_cmdq(data_array, 2, 1);
MDELAY(1);

data_array[0] = 0x00023902;
data_array[1] = 0x000060B1;
dsi_set_cmdq(data_array, 2, 1);
MDELAY(1);

data_array[0] = 0x00023902;
data_array[1] = 0x000000BD;
dsi_set_cmdq(data_array, 2, 1);
MDELAY(1);

data_array[0] = 0x00083902;
data_array[1] = 0x508140BF;
data_array[2] = 0x01FC1A00;
dsi_set_cmdq(data_array, 3, 1);
MDELAY(1);

data_array[0] = 0x00110500;
dsi_set_cmdq(data_array, 1, 1);
MDELAY(150);

data_array[0] = 0x00290500;
dsi_set_cmdq(data_array, 1, 1);
MDELAY(20);
}

 /* 
  static struct LCM_setting_table lcm_sleep_out_setting[] = {
      // Sleep Out
          {0x11, 0, {}},
      {REGFLAG_DELAY, 200, {}},
  
      // Display ON
          {0x29, 0, {}},
          {REGFLAG_END_OF_TABLE, 0x00, {}}
  };
 */ 
 /* 
  static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
          // Display off sequence
          {0x28, 0, {}},
  
      // Sleep Mode On
          {0x10, 0, {}},
          {REGFLAG_DELAY, 120, {}},
  
          {REGFLAG_END_OF_TABLE, 0x00, {}}
  };
 */ 
 /* 
  static struct LCM_setting_table lcm_backlight_level_setting[] = {
          {0x51, 1, {0xFF}},
          {REGFLAG_END_OF_TABLE, 0x00, {}}
  };
 */ 
 /* 
  static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
  {
      unsigned int i;
  
      for(i = 0; i < count; i++)
      {
          unsigned cmd;
          cmd = table[i].cmd;
  
          switch (cmd)
          {
              case REGFLAG_DELAY :
                  MDELAY(table[i].count);
                  break;
  
              case REGFLAG_END_OF_TABLE :
                  break;
  
              default:
                                  dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
          }
      }
  }
 */ 
  
  // ---------------------------------------------------------------------------
  //  LCM Driver Implementations
  // ---------------------------------------------------------------------------
  static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
  {
      memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
  }
  
    static void lcm_get_params(LCM_PARAMS *params)
{

		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

#if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
#else
		params->dsi.mode   = BURST_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE; 
#endif
	
		// DSI
		/* Command mode setting */
		//1 Three lane or Four lane
		params->dsi.LANE_NUM				= LCM_FOUR_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;
		params->dsi.packet_size=256;

		// Video mode setting		
		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		

               	params->dsi.vertical_sync_active				= 4;// 3    2
		params->dsi.vertical_backporch					= 14;// 20   1
		params->dsi.vertical_frontporch					= 15; // 1  12
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 50;// 50  2
		params->dsi.horizontal_backporch				= 50;
		params->dsi.horizontal_frontporch				= 50;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;


	    //params->dsi.LPX=8; 

		// Bit rate calculation
		params->dsi.PLL_CLOCK = 208;
		//esd check
  
  params->dsi.cont_clock=0;
  params->dsi.clk_lp_per_line_enable = 1;

   params->dsi.esd_check_enable =1;
   params->dsi.customization_esd_check_enable =1;
   params->dsi.lcm_esd_check_table[0].cmd = 0xd9;
   params->dsi.lcm_esd_check_table[0].count =1;
   params->dsi.lcm_esd_check_table[0].para_list[0] = 0x80;
  //params->dsi.lcm_esd_check_table[0].para_list[1] = 0x73;
	//params->dsi.clk_lp_per_line_enable = 0;

}
  
  
  static void lcm_init(void)
  {
          MDELAY(10);
          SET_RESET_PIN(1);
          MDELAY(5);
          SET_RESET_PIN(0);
          MDELAY(10);
  
          SET_RESET_PIN(1);
          MDELAY(180);
              
          init_lcm_registers_lide();
          
  }
  
  static void lcm_suspend(void)
  {
          unsigned int data_array[16];

          //data_array[0]=0x00280500; // Display Off
          //dsi_set_cmdq(data_array, 1, 1);
  
          data_array[0] = 0x00100500; // Sleep In
          dsi_set_cmdq(data_array, 1, 1);
          MDELAY(120);
          SET_RESET_PIN(1);
      SET_RESET_PIN(0);
      MDELAY(10);
      SET_RESET_PIN(1);
  
          MDELAY(10); 

      MDELAY(120);
  }
  
  static void lcm_resume(void)
  {
          lcm_init();
  }
  
  #if (LCM_DSI_CMD_MODE)
  static void lcm_update(unsigned int x, unsigned int y,
                         unsigned int width, unsigned int height)
  {
          unsigned int x0 = x;
          unsigned int y0 = y;
          unsigned int x1 = x0 + width - 1;
          unsigned int y1 = y0 + height - 1;
  
          unsigned char x0_MSB = ((x0>>8)&0xFF);
          unsigned char x0_LSB = (x0&0xFF);
          unsigned char x1_MSB = ((x1>>8)&0xFF);
          unsigned char x1_LSB = (x1&0xFF);
          unsigned char y0_MSB = ((y0>>8)&0xFF);
          unsigned char y0_LSB = (y0&0xFF);
          unsigned char y1_MSB = ((y1>>8)&0xFF);
          unsigned char y1_LSB = (y1&0xFF);
  
          unsigned int data_array[16];
  
          data_array[0]= 0x00053902;
          data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
          data_array[2]= (x1_LSB);
          dsi_set_cmdq(&data_array, 3, 1);
          
          data_array[0]= 0x00053902;
          data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
          data_array[2]= (y1_LSB);
          dsi_set_cmdq(&data_array, 3, 1);
  
          data_array[0]= 0x002c3909;
          dsi_set_cmdq(&data_array, 1, 0);
  
  }
  #endif
  
  static unsigned int lcm_compare_id(void)
{
	unsigned int id=0;
	unsigned char buffer[3];
	unsigned int array[16];  
        unsigned int lcd_id1=-1, lcd_id2 = -1;

	SET_RESET_PIN(1);
	MDELAY(10); 	
	SET_RESET_PIN(0);
	MDELAY(20);
	
	SET_RESET_PIN(1);
	MDELAY(80);  

	array[0] = 0x00043902; 
	array[1] = 0x9483ffb9;	
	dsi_set_cmdq(array, 2, 1);
	MDELAY(10); 

	array[0] = 0x00033700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);
	
	read_reg_v2(0x04, buffer, 3);
	id = buffer[0]; //we only need ID
        
        mt_set_gpio_mode(GPIO_LCD_MAKER_ID, GPIO_MODE_00);
        mt_set_gpio_dir(GPIO_LCD_MAKER_ID, GPIO_DIR_IN);
        mt_set_gpio_pull_enable(GPIO_LCD_MAKER_ID, 1);
        mt_set_gpio_pull_select(GPIO_LCD_MAKER_ID, GPIO_PULL_UP);
        lcd_id1 = mt_get_gpio_in(GPIO_LCD_MAKER_ID);

        mt_set_gpio_mode(GPIO_LCM_ID2_PIN, GPIO_MODE_00);
        mt_set_gpio_dir(GPIO_LCM_ID2_PIN, GPIO_DIR_IN);
        mt_set_gpio_pull_enable(GPIO_LCM_ID2_PIN, 1);
        mt_set_gpio_pull_select(GPIO_LCM_ID2_PIN, GPIO_PULL_UP);
        lcd_id2 =  mt_get_gpio_in(GPIO_LCM_ID2_PIN);

	
#ifndef BUILD_LK
 printk("kernel aeonlcm aeon_hx8394f_hd_dsi_vdo_e520_v552_xinli buffer[0] = 0x%x,buffer[1] = 0x%x,buffer[2] = 0x%x, lcd_id1=%d, lcd_id2=%d\n", buffer[0],buffer[1],buffer[2],lcd_id1,lcd_id2);
#else
 printf("kernel aeonlcm aeon_hx8394f_hd_dsi_vdo_e520_v552_xinli buffer[0] = 0x%x,buffer[1] = 0x%x,buffer[2] = 0x%x, lcd_id1=%d, lcd_id2=%d\n", buffer[0],buffer[1],buffer[2],lcd_id1,lcd_id2);
#endif

    if((buffer[0] == 0x83) && (lcd_id1 == 0) && (lcd_id2 == 0))
    {
    	return 1;
    }
    else
    {
        return 0;
    }

}
  
  static unsigned int lcm_esd_check(void)
  {
  #ifndef BUILD_LK
          char  buffer[3];
          int   array[4];
  
          array[0] = 0x00013700;
          dsi_set_cmdq(array, 1, 1);
  
          read_reg_v2(0x0a, buffer, 1);
 	  array[0] = 0x00013700;
 	  dsi_set_cmdq(array, 1, 1);
 	  read_reg_v2(0xd9, &buffer[1], 1);

   	printk("esd check buffer[0]=0x%x,buffer[1]=0x%x \n", buffer[0],buffer[1]); 
	  if((buffer[0]==0x1c)&&(buffer[1]==0x80))
          {
                  return 0;
          }
          else
          {
                  return 1;
          }
  #else
          return 0;
  #endif
  }
 
   static unsigned int lcm_esd_recover(void)
   {
        #ifndef BUILD_LK        
        lcm_init();
        #endif
        return 1;
                            
   }
  // ---------------------------------------------------------------------------
  //  Get LCM Driver Hooks
  // ---------------------------------------------------------------------------
  LCM_DRIVER aeon_hx8394f_hd720_dsi_vdo_lide_8536_lcm_drv = 
  {
      		.name         = "aeon_hx8394f_hd720_dsi_vdo_lide_8536",
          .set_util_funcs = lcm_set_util_funcs,
          .get_params     = lcm_get_params,
          .init           = lcm_init,
          .suspend        = lcm_suspend,
          .resume         = lcm_resume,
          .compare_id     = lcm_compare_id,
          .esd_check      = lcm_esd_check,
          .esd_recover    = lcm_esd_recover,
  #if (LCM_DSI_CMD_MODE)
      .update         = lcm_update,
  #endif
  };
