/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@42lyon.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 22:41:22 by gbaud             #+#    #+#             */
/*   Updated: 2021/01/06 01:41:37 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#define ERR_ARG "Error: argument\n"
#define ERR_OPE "Error: Operation file corrupted\n"

typedef struct  s_rect
{
    char        t;
    char        f;
    float       x;
    float       y;
    float       w;
    float       h;
}               t_rect;

int ft_strlen(char *str) {
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

int put_error(char *err) {
    write(1, err, ft_strlen(err));
    return (1);
}

int is_border(float x, float y, t_rect r) {
    if (x < r.x || x > r.x + r.w ||
        y < r.y || y > r.y + r.h)
        return (0);
    if ((x - r.x < 1.00000000 || (r.x + r.w) - x < 1.00000000) ||
        (y - r.y < 1.00000000 || (r.y + r.h) - y < 1.00000000))
        return (2);
    return (1);
}

int put_rect(int w, int h, char canvas[h][w], t_rect r) {
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            if (r.t == 'r') {
                if (is_border(x, y, r) == 2) canvas[y][x] = r.f;
            } else if (r.t == 'R') {
                if (is_border(x, y, r) > 0) canvas[y][x] = r.f;
            } else { return (1); }
    return (0);
}

int main(int ac, char **av) {
    FILE *operation_file;
    int w, h, last;
    t_rect r;
    char c;

    if (ac != 2)
        return (put_error(ERR_ARG));
    if (!(operation_file = fopen(av[1], "r")))
        return (put_error(ERR_OPE));

    last = fscanf(operation_file, "%d %d %c\n", &w, &h, &c);
    if (last != 3 || w <= 0 || w > 300 || h <= 0 || h > 300)
        return (put_error(ERR_OPE));

    char canvas[h][w];
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            canvas[y][x] = c;
    
    while (1) {
        last = fscanf(operation_file, "%c %f %f %f %f %c\n", &r.t, &r.x, &r.y, &r.w, &r.h, &r.f);
        if (last == EOF)
            break;
        else if (last != 6 || r.w <= 0 || r.h <= 0)
            return (put_error(ERR_OPE));
        else if (put_rect(w, h, canvas, r))
            return (put_error(ERR_OPE));
    }

    for (int y = 0; y < h; y++) {
        write(1, canvas[y], w);
        write(1, "\n", 1);
    }
    fclose(operation_file);
    return (0);
}