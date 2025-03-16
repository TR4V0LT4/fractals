/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wlahyani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 17:36:58 by wlahyani          #+#    #+#             */
/*   Updated: 2022/08/11 11:35:39 by wlahyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <mlx.h>
# include <stdio.h>
# include <unistd.h>
# include <math.h>
# include <stdlib.h>

#define NUM_THREADS 20
#define HEIGHT 1200
#define WIDTH 1200
#define KEY_UP 5
#define KEY_DOWN 4
#define KEY_LEFT 65361
#define KEY_RIGHT 65363
#define MAX_ITER 200

#include <mpfr.h>

typedef struct s_data{
    void *mlx;
    void *img;
    void *win;
    char *addr;
    char *t;

    int bits_per_pixel;
    int line_length;
    int endian;
    int x_offset;
    int y_offset;
    
    mpfr_t scale;  // Replace long double with mpfr_t for arbitrary precision
} t_data;

typedef struct s_cor{
    mpfr_t x;       // Arbitrary precision for coordinates
    mpfr_t y;
    mpfr_t r;
    mpfr_t i;
    mpfr_t c_r;
    mpfr_t c_i;
    int color;
    int iteration;
    long double row;
    long double col;
} t_cor;

typedef struct s_fractal {
    mpfr_t c_r;    // Complex constants with arbitrary precision
    mpfr_t c_i;
    int is_mandelbrot; // 1 if Mandelbrot, 0 if Julia
} t_fractal;


#include <pthread.h>  // Include POSIX threads

typedef struct s_thread_data
{
    t_data *img;
    int start_row;
    int end_row;
} t_thread_data;


void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
void	plan_complex(t_cor *p, t_data *img);
void	draw_fractal(t_cor *p, t_data *img);
void	guide(void);
void 	setup_fractal(t_fractal *frac, t_data *img);
int		zoom(int key, int x, int y, t_data *img);
int		ft_strncmp(char *s1, char *s2, int n);
int		calculate_iterations(t_cor p);
int		fill_image(t_data *img);
int		close_win(int key, void *p);
int		closex(int keycode, void *p);
int 	trippy_color(int iteration);
void 	*fill_part(void *arg);
int 	fill_image(t_data *img);


#endif
