typedef struct {
	const int *arr;
	int len;
	int target;
	int expected;   /* -1 se assente */
	const char *name;
} TestCase;

static const int arr0[] = {};
static const int arr1[] = {5};
static const int arr2[] = {1,2,3,4};
static const int arr3[] = {2,4,4,4,9};
static const int arr4[] = {1,3,5,7,9,11};
static const int arr5[] = {1,3,5,7,9,11,11,23,24,25,234,244,254,264,300,400,500,600};

static const TestCase TESTS[] = {
    {arr0, 0, 7,  -1, "vuoto"},
    {arr1, 1, 5,   0, "uno_match"},
    {arr1, 1, 7,  -1, "uno_no"},
    {arr2, 4, 1,   0, "inizio"},
    {arr2, 4, 4,   3, "fine"},
    {arr2, 4, 3,   2, "centro"},
    {arr2, 4, 5,  -1, "troppo_grande"},
    {arr2, 4, 0,  -1, "troppo_piccolo"},
    {arr3, 5, 4,  -2, "duplicati"}, /* -2 = qualsiasi occorrenza */
    {arr4, 6, 9,   4, "tipico"},
    {arr5, 18, 254,   12, "tipico"}
};
