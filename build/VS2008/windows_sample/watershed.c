#include "watershed.h"
#include <stdlib.h>
#include <string.h>

linked_contrast_t *contrast[256];
linked_region_t *root_region;
linked_pixel_t *border;

point_t *init_point(){
	point_t *p;

	p = (point_t *)malloc(sizeof(point_t));

	return p;
}

linked_pixel_t *init_linked_pixel(){
	linked_pixel_t *pixel;
	
	pixel = (linked_pixel_t *)malloc(sizeof(linked_pixel_t));
	pixel->p = init_point();
	pixel->next = NULL;
	
	return pixel;
}

linked_region_t *init_linked_region(){
	linked_region_t *region;
	//if (region == NULL) {	// 초기의 region이 과연 NULL인가?
		region = (linked_region_t *)malloc(sizeof(linked_region_t));
		region->next = NULL;
		region->lpx = init_linked_pixel();
	//}

	return region;
}

void insert_region_list(linked_pixel_t *lp){
	linked_region_t *lrg = (linked_region_t *)malloc(sizeof(linked_region_t));
	lrg->lpx = lp;

	if (root_region == NULL){
		root_region = lrg;
		lrg->next = NULL;
	} else {
		lrg->next = root_region;
		root_region = lrg;
	}
}

/**
 * @brief Insert Pixel list
 * 포인트 p와 인접한 링크드포인트 near_at_p_lpx를 가져와 near_at_p_lpx 다음에 p를 연결시킨다.
 * @param near_at_p_lpx		IN	Parameter, Mandatory, Linked-point near at the point
 * @param p					IN	Parameter, Mandatory, the point(x,y)
 */
void insert_pixel_list(linked_pixel_t *near_at_p_lpx, point_t *p){
	linked_pixel_t *lpx;
	lpx = init_linked_pixel();
	//lpx->p = p;		// 구조체에서 이런건 어떻게 하나?
	lpx->p->x = p->x;
	lpx->p->y = p->y;

	lpx->next = near_at_p_lpx->next;
	near_at_p_lpx->next = lpx;
}
void merge_pixel_list(linked_pixel_t *near_at_p_lpx, linked_pixel_t *head, linked_pixel_t *tail){
	if(near_at_p_lpx->next == NULL) {
		near_at_p_lpx->next = head;
	}
	else {
		tail->next = near_at_p_lpx->next;
		near_at_p_lpx->next = head;
	}
}

//void insert_dam(point_t *p){
//	linked_pixel_t *lpx;
//	lpx = init_linked_pixel();
//	lpx->p->x = p->x;
//	lpx->p->y = p->y;
//	if (border == NULL){	// 초기에 border는 NULL이 맞는가?
//		border = lpx;
//	} else {
//		lpx->next = border;
//		border = lpx;
//	}
//}

void insert_dam(linked_pixel_t *head, linked_pixel_t *tail){
	if (border == NULL){	// 초기에 border는 NULL이 맞는가?
		border = head;
	} else {
		tail->next = border->next;
		border->next = head;
	}
}

/* int i에 해당하는 contrast에 새로들어온 c를 연결
 * contrast[i]가 null이면 만들어 줄 것.
 * LIFO임
 */
void add_linked_contrast(int x, int y, int i){

	linked_contrast_t *ct = (linked_contrast_t *)malloc(sizeof(linked_contrast_t));
	ct->p = init_point();
	ct->p->x = x;
	ct->p->y = y;

	// init
	if (contrast[i] == NULL) {
		contrast[i] = ct;
		contrast[i]->next = NULL;
	}
	else
	{
		ct->next = contrast[i];
		contrast[i] = ct;
	}
}

linked_contrast_t *get_linked_contrast(int i){

	linked_contrast_t *ct;
	
	if (contrast[i] == NULL) {
		return NULL;
	}
	else
	{
		ct = contrast[i];
		contrast[i] = contrast[i]->next;
		return ct;
	}
}

/** 두 포인트의 인접성 검사
 * @result 1 = True, 2 = False
 * @param p1	IN, Mandatory, 1st point
 * @param p2	IN, Mandatory, 2nd point
 */
int adjoin_points(point_t *p1, point_t *p2){
	if((p1->x <= p2->x+1) && (p1->x >= p2->x-1) && (p1->y <= p2->y+1) && (p1->y >= p2->y-1)) {
		return TRUE;
	} else {
		return FALSE;
	}
}


//void compute_region(point_t *p, int width, int height){
//	int duplicate_count = 0;
//	linked_region_t *lrg;
//	linked_pixel_t *lpx;
//
//	linked_pixel_t *near_at_p_lpx = NULL;
//
//	lrg = root_region;
//
//	if (lrg == NULL)
//	{
//		// root가 없다면 바로 새로운 region 생성
//		insert_region_list(p);
//	} else {
//		do {
//			lpx = lrg->lpx;
//			do {
//				/* 기 존재하는 region의 인접한 픽셀일 경우 */
//				if((lpx->p->x <= p->x+1) && (lpx->p->x >= p->x-1) && (lpx->p->y <= p->y+1) && (lpx->p->y >= p->y-1)) {
//					duplicate_count++;	// 인접하는 픽셀 카운트 + 1
//					near_at_p_lpx = lpx;
//					break;
//				}
//				lpx = lpx->next;
//			} while (lpx != NULL);
//
//			if (duplicate_count > 1)	// 입력받은 point 주변 픽셀이 2개 이상이라면 끝냄
//				break;
//
//			lrg = lrg->next;
//		} while (lrg != NULL);
//
//		/* 인접한 region이 없는 경우 */
//		switch (duplicate_count)
//		{
//			case 0:
//				// 새로운 region 생성
//				insert_region_list(p);
//				break;
//			case 1:
//				// 기존 region에 해당 픽셀 포함시킴
//				insert_pixel_list(near_at_p_lpx, p);
//				break;
//			case 2:
//				// 댐 발생
//				insert_dam(p);
//				break;
//			default:
//				break;
//		}
//	}
//
//	duplicate_count = 0;
//}

void compute_region(linked_contrast_t *cont, int width, int height){
	linked_contrast_t *c;
	linked_contrast_t *prev_c;
	linked_region_t *lrg;
	linked_pixel_t *lpx;
	linked_pixel_t *lpx_for_cont;
	linked_pixel_t *head;
	linked_pixel_t *tail;
	int duplicate_count = 0;
	int adjoin_count = 0;
	int connect_flag = 0;

	linked_pixel_t *near_at_p_lpx = NULL;

	while(cont != NULL)
	{
		c = cont;
		head = init_linked_pixel();
		lpx = tail = head;
		head->p->x = c->p->x;
		head->p->y = c->p->y;
		adjoin_count = 1;
		while(adjoin_count > 0){
			adjoin_count = 0;
			lpx = head;
			while (lpx != NULL){
				prev_c = cont;
				c = cont->next;
				while (c != NULL)
				{
					if(adjoin_points(lpx->p, c->p))
					{
						insert_pixel_list(tail, c->p);
						if (tail->next != NULL)
							tail = tail->next;
						prev_c->next = c->next;
						free(c);	// 연결된 constant free 시킴
						c = prev_c->next;
						adjoin_count++;
					} else {
						prev_c = c;
						c = c->next;
					}
				}
				lpx = lpx->next;
			}
		}

		c = cont;
		cont = cont->next;
		free(c);	// head에 포함된 constant free시킴
		c = NULL;


		// watershed 알고리즘 적용하기 시작
		// 현재 constant에서 연결된 덩어리 watershed 알고리즘 적용하기
		lrg = root_region;
		if (lrg == NULL)
		{
			// root가 없다면 바로 새로운 region 생성
			insert_region_list(head);
		} else {
			do {
				lpx = lrg->lpx;
				do {
					lpx_for_cont = head;
					do 
					{
						/* 기 존재하는 region의 인접한 픽셀일 경우 */
						if(adjoin_points(lpx->p, lpx_for_cont->p)) {
							connect_flag = 1;
							near_at_p_lpx = lpx;
							break;
						}
						lpx_for_cont = lpx_for_cont->next;
					} while (lpx_for_cont != NULL);
					if (connect_flag == 1) {
						duplicate_count++;	// 인접하는 픽셀 카운트 + 1
						connect_flag = 0;
						break;
					}
					lpx = lpx->next;
				} while (lpx != NULL);

				if (duplicate_count > 1)	// 입력받은 point 주변 픽셀이 2개 이상이라면 끝냄
					break;

				lrg = lrg->next;
			} while (lrg != NULL);

			/* 인접한 region이 없는 경우 */
			switch (duplicate_count)
			{
			case 0:
				// 새로운 region 생성
				insert_region_list(head);
				break;
			case 1:
				// 기존 region에 해당 픽셀 포함시킴
				merge_pixel_list(near_at_p_lpx, head, tail);
				break;
			case 2:
				// 댐 발생
				insert_dam(head, tail);
				break;
			default:
				break;
			}
		}

		duplicate_count = 0;

		// watershed 알고리즘 적용하기 끝.
	}

}

int watershed_operation(unsigned char *src, unsigned char *dst, int width, int height){
	int v, h, i;
	//linked_contrast_t *c;
	linked_region_t *lrg;
	linked_pixel_t *lpx;

	linked_region_t *lrg_for_free;
	linked_pixel_t *lpx_for_free;
	
	// 명암순서대로 배열에 큐잉
	for (v = 0; v < height; v++)
	{
		for (h = 0; h < width; h++)
		{
			add_linked_contrast(h, v, src[v*width+h]);
		}
	}
	
	for (i = 0; i < 256; i++)
	{
		//c = get_linked_contrast(i);
		//while(c != NULL){	// i에 해당하는 level의 명암을 가지는 픽셀을 모두 dequeue하고나면 i+1을 dequeue하는 형태.
		//	compute_region(c->p, width, height);

		//	free(c); //
		//	c = get_linked_contrast(i); // 마지막 dequeue는 null이므로 free뒤에 와야 함.
		//}

		compute_region(contrast[i], width, height);
		if (contrast[i]!=NULL) {
			//free(contrast[i]);
			contrast[i] = NULL;
		}
	}

	lrg = root_region;
	while(lrg != NULL) {
		lpx = lrg->lpx;
		while(lpx != NULL) {
			v = lpx->p->y;
			h = lpx->p->x;
			dst[v*width+h] = 0;	// region은 검은색
			lpx_for_free = lpx;
			lpx = lpx->next;
			free(lpx_for_free);
			lpx_for_free = NULL;
		}
		lrg_for_free = lrg;
		lrg = lrg->next;
		free(lrg_for_free);
		lrg_for_free = NULL;
	}

	lpx = border;
	while (lpx != NULL) {
		v = lpx->p->y;
		h = lpx->p->x;
		dst[v*width+h] = 255;	// border는 흰색
		lpx_for_free = lpx;
		lpx = lpx->next;
		free(lpx_for_free);
		lpx_for_free = NULL;
	}

	root_region = NULL;
	border = NULL;

	return 0;
}