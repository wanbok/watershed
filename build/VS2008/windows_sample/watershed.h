#define TRUE 1
#define FALSE 0

// 한 픽셀의좌표
typedef struct point
{
	int x, y;
} point_t;

// 한 region의 좌표 집합
typedef struct linked_pixel
{
	struct point *p;
	struct linked_pixel *next;
} linked_pixel_t;

// 전체 region들의 집합
typedef struct linked_region
{
	//	unsigned int left, right, top, bottom;
	struct linked_pixel *lpx;
	struct linked_region *next;
} linked_region_t;

// contrast queue
typedef struct linked_contrast
{
	struct point *p;
	struct linked_contrast *next;
} linked_contrast_t;

void add_linked_contrast(int x, int y, int i);
linked_contrast_t *get_linked_contrast(int i);
linked_pixel_t *init_linked_pixel();
linked_region_t *init_linked_region();
void insert_region_list(linked_pixel_t *lp);
/**
 * @brief Insert Pixel list
 * 포인트 p와 인접한 링크드포인트 near_at_p_lpx를 가져와 near_at_p_lpx 다음에 p를 연결시킨다.
 * @param near_at_p_lpx		IN	Parameter, Mandatory, Linked-point near at the point
 * @param p					IN	Parameter, Mandatory, the point(x,y)
 */
void insert_pixel_list(linked_pixel_t *near_at_p_lpx, point_t *p);
void merge_pixel_list(linked_pixel_t *near_at_p_lpx, linked_pixel_t *head, linked_pixel_t *tail);
//void insert_dam(point_t *p);
void insert_dam(linked_pixel_t *head, linked_pixel_t *tail);
void compute_region(linked_contrast_t *p, int width, int height);
int watershed_operation(unsigned char *src, unsigned char *dst, int width, int height);