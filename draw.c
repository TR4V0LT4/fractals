/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wlahyani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 15:52:41 by wlahyani          #+#    #+#             */
/*   Updated: 2022/08/09 16:02:24 by wlahyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fractol.h"

void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int *) dst = color;
}

void plan_complex(t_cor *p, t_data *img)
{
    // Arbitrary precision scale calculation
    mpfr_t scale_x, scale_y;
    mpfr_init2(scale_x, 256);  // Set precision for scale_x
    mpfr_init2(scale_y, 256);  // Set precision for scale_y

    // scale_x = (4 * img->scale) / WIDTH
    mpfr_mul_d(scale_x, img->scale, 4.0, MPFR_RNDN);  // 4 * img->scale
    mpfr_div_d(scale_x, scale_x, WIDTH, MPFR_RNDN);    // scale_x = (4 * img->scale) / WIDTH

    // scale_y = (4 * img->scale) / HEIGHT
    mpfr_mul_d(scale_y, img->scale, 4.0, MPFR_RNDN);  // 4 * img->scale
    mpfr_div_d(scale_y, scale_y, HEIGHT, MPFR_RNDN);    // scale_y = (4 * img->scale) / HEIGHT

    // r = (col - x_offset) * scale_x
    mpfr_t tmp_r, tmp_col, tmp_x_offset;
    mpfr_init2(tmp_r, 256);
    mpfr_init2(tmp_col, 256);
    mpfr_init2(tmp_x_offset, 256);

    mpfr_set_d(tmp_col, p->col, MPFR_RNDN);          // col
    mpfr_set_d(tmp_x_offset, img->x_offset, MPFR_RNDN);  // x_offset

    mpfr_sub(tmp_col, tmp_col, tmp_x_offset, MPFR_RNDN);  // (col - x_offset)
    mpfr_mul(tmp_r, tmp_col, scale_x, MPFR_RNDN);         // (col - x_offset) * scale_x
    mpfr_set(p->r, tmp_r, MPFR_RNDN);                     // Assign result to p->r

    // i = (row - y_offset) * scale_y
    mpfr_t tmp_i, tmp_row, tmp_y_offset;
    mpfr_init2(tmp_i, 256);
    mpfr_init2(tmp_row, 256);
    mpfr_init2(tmp_y_offset, 256);

    mpfr_set_d(tmp_row, p->row, MPFR_RNDN);          // row
    mpfr_set_d(tmp_y_offset, img->y_offset, MPFR_RNDN);  // y_offset

    mpfr_sub(tmp_row, tmp_row, tmp_y_offset, MPFR_RNDN);  // (row - y_offset)
    mpfr_mul(tmp_i, tmp_row, scale_y, MPFR_RNDN);         // (row - y_offset) * scale_y
    mpfr_set(p->i, tmp_i, MPFR_RNDN);                     // Assign result to p->i

    // Clean up temporary variables
    mpfr_clear(scale_x);
    mpfr_clear(scale_y);
    mpfr_clear(tmp_r);
    mpfr_clear(tmp_col);
    mpfr_clear(tmp_x_offset);
    mpfr_clear(tmp_i);
    mpfr_clear(tmp_row);
    mpfr_clear(tmp_y_offset);
}

void draw_fractal(t_cor *p, t_data *img)
{
    p->color = (p->iteration < MAX_ITER) ? trippy_color(p->iteration) : 0x000000;
    my_mlx_pixel_put(img, p->col, p->row, p->color);
}

int fill_image(t_data *img)
{
    pthread_t threads[NUM_THREADS];
    t_thread_data thread_data[NUM_THREADS];
    int rows_per_thread = HEIGHT / NUM_THREADS;

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].img = img;
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i == NUM_THREADS - 1) ? HEIGHT : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, fill_part, &thread_data[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    // Display the completed image
    mlx_put_image_to_window(img->mlx, img->win, img->img, 0, 0);
    return (0);
}

void *fill_part(void *arg)
{
    t_thread_data *data = (t_thread_data *)arg;
    t_cor p;
    t_fractal frac;

    setup_fractal(&frac, data->img); // Set up Mandelbrot or Julia set

    for (p.row = data->start_row; p.row < data->end_row; p.row++)
    {
        for (p.col = 0; p.col < WIDTH; p.col++)
        {
            plan_complex(&p, data->img);

            if (frac.is_mandelbrot)
            {
                mpfr_set(p.c_r, p.r, MPFR_RNDN);
                mpfr_set(p.c_i, p.i, MPFR_RNDN);
                mpfr_set_d(p.x, 0, MPFR_RNDN);
                mpfr_set_d(p.y, 0, MPFR_RNDN);
            }
            else
            {
                mpfr_set(p.x, p.r, MPFR_RNDN);
                mpfr_set(p.y, p.i, MPFR_RNDN);
                mpfr_set(p.c_r, frac.c_r, MPFR_RNDN);
                mpfr_set(p.c_i, frac.c_i, MPFR_RNDN);
            }

            p.iteration = calculate_iterations(p);
            draw_fractal(&p, data->img);
        }
    }
    return NULL;
}
