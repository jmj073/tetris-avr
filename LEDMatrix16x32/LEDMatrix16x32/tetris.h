/*
 * tetris.h
 *
 * Created: 2022-07-05 22:09:14
 *  Author: JMJ
 */ 

/*
 * @NOTE stdlib 의 rand 를 사용하므로 srand 호출 필요
 */

#ifndef TETRIS_H_
#define TETRIS_H_

#include "display_matrix_api.h"

void tetris_init();
void tetris_process_input(u8 curr_input, u8 prev_input);
u8 tetris_do_tick();
void draw_score(coord_t row);

#endif /* TETRIS_H_ */