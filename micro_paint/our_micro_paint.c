/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_micro_paint.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@42lyon.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 12:17:45 by gbaud             #+#    #+#             */
/*   Updated: 2021/01/05 13:45:16 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

#define ERR_OPE "Error: Operation file corrupted\n"
#define ERR_ARG "Error: argument\n"

int ft_strlen(char *str) {
	int i = 0;
	while (str[i])
		i++;
	return (i);
} 

int put_error(char *err) {
	write(1, err, ft_strlen(err));
	return (1);
}

int ft_round(float f) {
	float inf = (int)f;
	float sup = (int)f + 1;
	return ((int)((f - inf) < (sup - f) ? inf : sup));
}

void put_rect(int w, int h, char canevas[h][w], char t, float rx, float ry, float rw, float rh, char f) {
	for (int y = (ry < 0) ? 0 : ry; y <= ry + rh && y < h; y++)
		for (int x = (rx < 0) ? 0 : rx; x <= rx + rw  && x < w; x++)
			if (t == 'r' && (x == rx || x == rx + rw || y == ry || y == ry + rh))
				canevas[y][x] = f;
			else if (t == 'R')
				canevas[y][x] = f;
}

void put_map(int w, int h, char canevas[h][w]) {
	for (int y = 0; y < h; y++) {
		write(1, canevas[y], w);
		write(1, "\n", 1);
	}
}

int main(int ac, char *av[])
{
	(void)av;
	if (ac != 2)
		return (put_error(ERR_ARG));

	FILE *instruction;
	if (!(instruction = fopen(av[1], "r")))
		return (put_error(ERR_OPE));

	int w, h;
	char c;
	fscanf(instruction, "%d %d %c", &w, &h, &c);

	char canevas[h][w];
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			canevas[y][x] = c;

	char t, f;
	float rx, ry, rw, rh;
	while (fscanf(instruction, "\n%c %f %f %f %f %c", &t, &rx, &ry, &rw, &rh, &f) != EOF) {
		if ((int)rx != rx) rx = (int)rx + 1;
		if ((int)ry != ry) ry = (int)ry + 1;
		rw = ((int)rw != rw) ? rw - 1 : rw - 1;
		rh = ((int)rh != rh) ? rh - 1 : rh - 1;
		dprintf(1, "%f %f %f %f\n", rx, ry, rw, rh);
		put_rect(w, h, canevas, t, rx, ry, rw, rh, f);
	}
	put_map(w, h, canevas);
	return (0);
}
