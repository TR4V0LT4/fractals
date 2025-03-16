/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractals.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wlahyani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 16:02:59 by wlahyani          #+#    #+#             */
/*   Updated: 2022/08/09 16:08:19 by wlahyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fractol.h"

void mpfr_init_precision(mpfr_t var) {
    mpfr_init2(var, 256);  // Use 256 bits precision for arbitrary-precision calculations
}

// Absolute value function using MPFR
void my_fabsl(mpfr_t result, mpfr_t value) {
    mpfr_abs(result, value, MPFR_RNDN);  // Absolute value of value, result stores the result
}

// Calculate iterations with arbitrary precision using MPFR
int calculate_iterations(t_cor *p)
{
    mpfr_t temp, x_squared_plus_y_squared;

    // Initialize variables with 256-bit precision
    mpfr_init_precision(temp);
    mpfr_init_precision(x_squared_plus_y_squared);

    p->iteration = 0;

    // Perform the iteration until the condition is met
    while (1) {
        // x^2 + y^2
        mpfr_mul(x_squared_plus_y_squared, p->x, p->x, MPFR_RNDN);  // x^2
        mpfr_t y_squared;
        mpfr_init_precision(y_squared);
        mpfr_mul(y_squared, p->y, p->y, MPFR_RNDN);  // y^2
        mpfr_add(x_squared_plus_y_squared, x_squared_plus_y_squared, y_squared, MPFR_RNDN);  // x^2 + y^2

        // Check if the value is less than 4 (x^2 + y^2 < 4)
        if (mpfr_cmp_d(x_squared_plus_y_squared, 4) >= 0 || p->iteration >= MAX_ITER)
            break;

        // Compute the new x and y values (Mandelbrot/Julia formulas)
        mpfr_mul(temp, p->x, p->x, MPFR_RNDN);  // x^2
        mpfr_sub(temp, temp, p->y, MPFR_RNDN);  // x^2 - y
        mpfr_add(temp, temp, p->c_r, MPFR_RNDN);  // (x^2 - y) + c_r
        mpfr_set(p->x, temp, MPFR_RNDN);  // x = temp

        mpfr_mul(temp, p->x, p->y, MPFR_RNDN);  // 2 * x * y
        mpfr_add(p->y, temp, p->c_i, MPFR_RNDN);  // y = (2 * x * y) + c_i

        p->iteration++;
    }

    mpfr_clear(temp);
    mpfr_clear(x_squared_plus_y_squared);
    return p->iteration;
}

// Setup fractal settings based on type
void setup_fractal(t_fractal *frac, t_data *img)
{
    if (!ft_strncmp("M", img->t, 1))  // Mandelbrot
        frac->is_mandelbrot = 1;
    else  // Julia Variants
    {
        frac->is_mandelbrot = 0;

        // Setup Julia set values based on input type
        if (!ft_strncmp("J", img->t, 2)) {
            mpfr_set_d(frac->c_r, -0.4, MPFR_RNDN);  // c_r = -0.4
            mpfr_set_d(frac->c_i, 0.6, MPFR_RNDN);   // c_i = 0.6
        }
        else if (!ft_strncmp("J1", img->t, 2)) {
            mpfr_set_d(frac->c_r, 0.28, MPFR_RNDN);  // c_r = 0.28
            mpfr_set_d(frac->c_i, 0.008, MPFR_RNDN); // c_i = 0.008
        }
        else if (!ft_strncmp("J2", img->t, 2)) {
            mpfr_set_d(frac->c_r, -0.79, MPFR_RNDN); // c_r = -0.79
            mpfr_set_d(frac->c_i, 0.15, MPFR_RNDN);  // c_i = 0.15
        }
        else if (!ft_strncmp("J3", img->t, 2)) {
            mpfr_set_d(frac->c_r, -0.162, MPFR_RNDN); // c_r = -0.162
            mpfr_set_d(frac->c_i, 1.04, MPFR_RNDN);  // c_i = 1.04
        }
    }
}