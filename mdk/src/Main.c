#include "Main.h"

int main(void)
{
	debug_idicator_light_init();
	ws2812b_init();
	hc06_init();
	flag_init();
	debug_idicator_light_flash(100, 5);
	bool ws2812b_power = true;
	debug_idicator_light_level_set(ws2812b_power);
//	while (true)
//		ws2812b_meteor(ws2812b_color(0xFF, 0x00, 0x00), 36, 6, 3);
	
	while (true)
	{	
		// delay_ms(500);
		cmd_capture();
		
//		if (cmd_field_check(0, "/help"))
//		{
//			printf("/help					Display instructions with details.\n");
//			printf("/power [on / off]		Turn on/off the light.\n");
//			printf("/breath <RGB-Code>		Breath as the specified RGB color.\n");
//			printf("<RGB-Code>				- Apply white as defult color if the argument is not declared.\n");
//			printf("						- Example: /breath FFFF00:	Breath as the color set to yellow.\n");
//			printf("/rainbow <flow>			Colorful Flashing.\n");
//			printf("<flow>					- Colorful and flowing flashing.");
//			printf("						- Static flowing without the argument.\n");
//			printf("Tip: The optional arguments are covered with \"<>\" and the necessary are covered with \"[]\".");
//			cmd_head_clear();
//			continue;
//		}
//		else if (cmd_field_check(0, "/power"))
		if (cmd_field_check(0, "/power"))
		{
			if (cmd_field_check(1, "on"))
			{
				ws2812b_power = true;
				debug_idicator_light_level_set(ws2812b_power);
				printf("[Power]ON\n");
			}
			else if (cmd_field_check(1, "off"))
			{
				ws2812b_power = false;
				debug_idicator_light_level_set(ws2812b_power);
				printf("[Power]OFF\n");
			}
			else
				printf("[Power]Incorrect argument\n");
			flag_storage();
			cmd_head_clear();
			continue;
		}
		else if (cmd_field_check(0, "/color"))
		{
			val_color = cmd_field_rgb_value(1);
			printf("[Color]RGB(%06X)\n", val_color);
			flag_storage();
			cmd_head_clear();
			continue;
		}
		else if (cmd_field_check(0, "/default"))
		{		
			flag_reset_all();
			flag_default = true;
			val_color = cmd_field_rgb_value(1) ? cmd_field_rgb_value(1) : val_color;
			printf("[Default]RGB(%06X)\n", val_color);
			flag_storage();
			cmd_head_clear();
			continue;
		}
		else if (cmd_field_check(0, "/breath"))
		{		
			flag_reset_all();
			flag_breath = true;
			val_color = cmd_field_rgb_value(1) ? cmd_field_rgb_value(1) : val_color;
			printf("[Breath]RGB(%06X)\n", val_color);
			flag_storage();
			cmd_head_clear();
			continue;
		}
		else if (cmd_field_check(0, "/meteor"))
		{
			flag_reset_all();
			flag_meteor = true;
			val_color = cmd_field_rgb_value(1);
			val_len = cmd_field_value(2);
			val_interval = cmd_field_value(3);
			val_wait = cmd_field_value(4);
			printf("[Meteor]RGB(%06X)\n", val_color);
			flag_storage();
			cmd_head_clear();
			continue;
		}
		else if (cmd_field_check(0, "/rainbow"))
		{
			flag_reset_all();
			if (cmd_field_check(1, "flow"))
			{
				printf("[Rainbow]Flowing\n");
				flag_rainbow_flow = true;
			}
			else
			{
				printf("[Rainbow]Static\n");
				flag_rainbow = true;
			}
			flag_storage();
			cmd_head_clear();
			continue;
		}
		else 
			cmd_head_clear();
		
		if (ws2812b_power)
		{
			if (flag_default)
				ws2812b_color_wipe(ws2812b_color(val_color >> 16, val_color << 16 >> 24, val_color << 24 >> 24), 0);
			else if (flag_breath)
				ws2812b_breath(PIXEL_NUM, val_color >> 16, val_color << 16 >> 24, val_color << 24 >> 24, 20);
			else if (flag_meteor)
				ws2812b_meteor(val_color, val_len, val_interval, val_wait);
			else if (flag_rainbow)
				ws2812b_rainbow_with_static(10);
			else if (flag_rainbow_flow)
				ws2812b_rainbow_cycle(10);
		}
		else
			ws2812b_color_wipe(0, 0);
	}
	
	return 0;
}
