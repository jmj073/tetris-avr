/*
 * tetris.h
 *
 * Created: 2022-07-05 22:09:14
 *  Author: JMJ
 */ 

/*
 * @NOTE stdlib �� rand �� ����ϹǷ� srand ȣ�� �ʿ�
 */

#ifndef TETRIS_H_
#define TETRIS_H_

#include "display_matrix_api.h"

void tetris_init();
void tetris_process_input(u8 curr_input, u8 prev_input);
u8 tetris_do_tick();
void draw_score(coord_t row);

#endif /* TETRIS_H_ */