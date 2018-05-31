/* $LastChangedDate: 2017-09-10 17:40:40 +0200 (Sun, 10 Sep 2017) $ */
/*
 DummyDisplayDevice.h : virtual device Fim driver header file

 (c) 2008-2017 Michele Martone

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef FIM_DUMMYDISPLAY_DEVICE_H
#define FIM_DUMMYDISPLAY_DEVICE_H


class DummyDisplayDevice FIM_FINAL:public DisplayDevice
{
	/*
	 * The generalization of a Fim output device.
	 */
	public:
	virtual fim_err_t initialize(sym_keys_t &sym_keys){return FIM_ERR_NO_ERROR;}
	virtual void  finalize(void){}

	virtual fim_err_t display(
		const void *ida_image_img, // source image structure
		fim_coo_t iroff,fim_coo_t icoff, // row and column offset of the first input pixel
		fim_coo_t irows,fim_coo_t icols,// rows and columns in the input image
		fim_coo_t icskip,	// input columns to skip for each line
		fim_coo_t oroff,fim_coo_t ocoff,// row and column offset of the first output pixel
		fim_coo_t orows,fim_coo_t ocols,// rows and columns to draw in output buffer
		fim_coo_t ocskip,// output columns to skip for each line
		fim_flags_t flags// some flags
		){return FIM_ERR_NO_ERROR;}

#ifndef FIM_WANT_NO_OUTPUT_CONSOLE
	DummyDisplayDevice(MiniConsole& mc_):DisplayDevice(mc_){}
#else
	DummyDisplayDevice(void){}
#endif /* FIM_WANT_NO_OUTPUT_CONSOLE */
	virtual ~DummyDisplayDevice(void){}

	virtual fim_coo_t get_chars_per_line(void)const{return 0;/* this is a special value */}
	virtual fim_coo_t get_chars_per_column(void)const{return 0;/* */}
	virtual fim_coo_t width(void)const{return 1;/* 0 would be so cruel */}
	virtual fim_coo_t height(void)const{return 1;/* 0 would be so cruel */}
	virtual fim_err_t status_line(const fim_char_t *msg){return FIM_ERR_NO_ERROR;}
	virtual fim_err_t console_control(fim_cc_t code){return FIM_ERR_NO_ERROR;}
	virtual fim_bool_t handle_console_switch(void){return false;}
	virtual fim_err_t clear_rect(fim_coo_t x1, fim_coo_t x2, fim_coo_t y1,fim_coo_t y2){return FIM_ERR_NO_ERROR;}
	virtual fim_err_t fill_rect(fim_coo_t x1, fim_coo_t x2, fim_coo_t y1,fim_coo_t y2, fim_color_t color){ return FIM_ERR_NO_ERROR; }
	fim_err_t fs_puts(struct fs_font *f, fim_coo_t x, fim_coo_t y, const fim_char_t *str){return FIM_ERR_NO_ERROR;}
	virtual fim_bpp_t get_bpp(void)const{return 0;/* 0 signifies non-graphical output */}
	virtual fim_coo_t status_line_height(void)const{return 0;}

	private:
};

#endif /* FIM_DUMMYDISPLAY_DEVICE_H */
