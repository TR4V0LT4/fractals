/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wlahyani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 10:59:08 by wlahyani          #+#    #+#             */
/*   Updated: 2022/08/11 11:03:11 by wlahyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fractol.h"

// #include "fractol.h"
// #include <mpfr.h>

int zoom(int button, int x, int y, t_data *img)
{
    if (button != KEY_DOWN && button != KEY_UP)
        return (0);

    mpfr_t zoom_factor, mouse_r, mouse_i, temp_x, temp_y;

    // Initialize with 256-bit precision
    mpfr_init2(zoom_factor, 256);
    mpfr_init2(mouse_r, 256);
    mpfr_init2(mouse_i, 256);
    mpfr_init2(temp_x, 256);
    mpfr_init2(temp_y, 256);

    // Set zoom factor based on button press (down or up)
    if (button == KEY_DOWN)
        mpfr_set_d(zoom_factor, 1.1, MPFR_RNDN);  // Zoom out (increase scale)
    else
        mpfr_set_d(zoom_factor, 0.9, MPFR_RNDN);  // Zoom in (decrease scale)

    // Calculate mouse position in terms of the fractal space with precision
    mpfr_sub(temp_x, x, img->x_offset, MPFR_RNDN);  // x - x_offset
    mpfr_mul(mouse_r, temp_x, img->scale, MPFR_RNDN); // (x - x_offset) * scale

    mpfr_sub(temp_y, y, img->y_offset, MPFR_RNDN);  // y - y_offset
    mpfr_mul(mouse_i, temp_y, img->scale, MPFR_RNDN); // (y - y_offset) * scale

    // Multiply scale by zoom factor
    mpfr_mul(img->scale, img->scale, zoom_factor, MPFR_RNDN);

    // Ensure scale is not zero
    if (mpfr_eq_d(img->scale, 0.0, MPFR_RNDN))
        mpfr_set_d(img->scale, 1.0 / 6.0, MPFR_RNDN);  // Default small scale value if it becomes zero

    // Update offsets based on the zoom and mouse position
    mpfr_div(temp_x, mouse_r, img->scale, MPFR_RNDN);
    mpfr_div(temp_y, mouse_i, img->scale, MPFR_RNDN);

    mpfr_sub(img->x_offset, x, temp_x, MPFR_RNDN);  // x - (mouse_r / scale)
    mpfr_sub(img->y_offset, y, temp_y, MPFR_RNDN);  // y - (mouse_i / scale)

    // Clear the MPFR variables to avoid memory leaks
    mpfr_clear(zoom_factor);
    mpfr_clear(mouse_r);
    mpfr_clear(mouse_i);
    mpfr_clear(temp_x);
    mpfr_clear(temp_y);

    return (0);
}

int close_win(int key, void *p)
{
    (void)p;
    if (key == 53)  // Key 53 is typically the Escape key on macOS
        exit(1);
    return (0);
}

int closex(int keycode, void *p)
{
    (void)keycode;
    (void)p;
    exit(1);
    return (0);
}
