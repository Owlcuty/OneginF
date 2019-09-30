#define ERRFILE 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

#include <assert.h>



const int MaxStr = 300;
const int MaxLines = 7000;
const int MaxArr = 100000;

struct Line
{
  char* str;
  int len;
  int order;
};

void set_iter(int arr[], int num_elem);

void swap(int a1, int a2);
int strcompare(const void* l1, const void* l2);
int strcompareBack(const void* l1, const void* l2);

int readTextRows(int positions[], FILE* text);
// char* inputText(int fd, int size_file);
char* inputText(FILE* ftext, int size_file);
int settingLines(char* text, Line lines[]);

void writeText(FILE* fout, Line lines[], int num_elem);

void heapify(int ptrarr[], char* text, int positions[], int num_elem, int v);

void build_heap(int ptrarr[], char* text, int positions[], int num_elem);

void heap_sort(int ptrarr[], char* text, int positions[], int num_elem);


bool compareStr(Line l1, Line l2);
bool backCompareStr(int ptr1, int ptr2, int positions[], char* text);

void quickSort(Line lines[], int left, int right, int num_elem, bool (*compare)(Line l1, Line l2));

int main()
{
  // int OneginF = 0;
  // if ((OneginF = open("origOnegin", O_RDONLY, 0)) == -1)
  FILE* OneginF = NULL;
  if ((OneginF = fopen("origOnegin", "r")) == NULL)
  {
    printf("%i:: Can't open readable file" "\n", __LINE__);
    return ERRFILE;
  }
  // int Encyclopedia = 0;
  // if ((Encyclopedia = open("encyclOnegin", O_WRONLY)) == -1)
  FILE* Encyclopedia = NULL;
  if ((Encyclopedia = fopen("encOnegin", "w")) == NULL)
  {
    printf("%i:: Can't open writable file" "\n", __LINE__);
    return ERRFILE;
  }

  fseek(OneginF, 0, SEEK_END);
  long size_file = 0;
  if ((size_file = ftell(OneginF)) == -1L)
  {
    printf("%i:: Error with moving in OneginF" "\n", __LINE__);
  }
  fseek(OneginF, 0, SEEK_SET);

  char* onegin = inputText(OneginF, size_file);

  Line* lines = (Line*)(calloc(MaxLines, sizeof(*lines)));

  int num_rows = 0;
  if ((num_rows = settingLines(onegin, lines)) == -1)
  {
    printf("%i:: Error with counting lines" "\n", __LINE__);
  }
  // heap_sort(ptrarr, text, positions, num_rows);

printf("%i:: Working" "\n", __LINE__);
printf("%i:: Working" "\n", __LINE__);

  // quickSort(lines, 0, num_rows - 1, num_rows, compareStr);
  qsort(lines, num_rows, sizeof(Line), strcompare);
printf("%i:: Working. END -------------------------" "\n", __LINE__);

  writeText(Encyclopedia, lines, num_rows);
  qsort(lines, num_rows, sizeof(Line), strcompareBack);
  writeText(Encyclopedia, lines, num_rows);

  fprintf(Encyclopedia, onegin);

  // for (int ch = 0; ch < 20; ch ++)
  // {
    // fprintf(Encyclopedia, "-");
  // }
  // fprintf(Encyclopedia, "\n");

  // quickSort(ptrarrRhyme, positions, text, 0, num_rows - 1, num_rows, backCompareStr);

  // writeText(Encyclopedia, ptrarrRhyme, positions, num_rows, text);

  // writeText(Encyclopedia, ptrarr, positions, num_rows, text);

  // free(text);
  // free(ptrarr);
  // free(ptrarrSort);
  // free(ptrarrRhyme);
  //

  free(onegin);
  free(lines);

  fclose(OneginF);
  fclose(Encyclopedia);
}

void set_iter(int array[], int num_elem)
{
  for (int it = 0; it < num_elem; it ++)
  {
    array[it] = it;
  }
}

bool is_letter(char chr)
{
  if (('a' <= chr && chr <= 'z') || ('A' <= chr && chr <= 'Z'))
  {
    return true;
  }
  return false;
}

char toLower(char chr)
{
  // if (!is_letter(chr))
  // {
  //   printf("%i:: Chr = %c\n", __LINE__, chr);
  // }
  assert(is_letter(chr));

  return ('a' <= chr && chr <= 'z') ? chr : chr + 'a' - 'A';
}

void swap(int* a1, int* a2)
{
  int temp = (*a1);
  (*a1) = (*a2);
  (*a2) = temp;
}

int settingLines(char* text, Line lines[])
{
  assert(text);
  assert(lines);

  printf("%i:: Working" "\n", __LINE__);
  int last_line = 0;
  int slen = strlen(text);

  for (char* s = text; s < text + slen; s ++)
  {
    lines[last_line].str   = s;
    lines[last_line].order = last_line;
    lines[last_line].len   = -1;
    if ((s = (char*)(memchr(s, '\n', slen))) == NULL)
    {
      printf("%i:: Wrong memchr" "\n", __LINE__);
      return -1;
    }
    lines[last_line].len = s - lines[last_line].str;
    ++ last_line;
  }

printf("%i:: Working" "\n", __LINE__);
  return last_line ? last_line : -1;
}


// -- Comparators -----------------------------------------------------

/* int strcompare(Line l1, Line l2)
{
  
} */

int strcompare(const void* a, const void* b)
{
  assert(a && b);

  const Line *l1 = (Line const *) a;
  const Line *l2 = (Line const *) b;

  char const * l1_end = l1->str + l1->len;
  char const * l2_end = l2->str + l2->len;

  for (char *s1 = l1->str, *s2 = l2->str; s1 < l1_end && s2 < l2_end; s1 ++, s2 ++)
  {

    while (s1 < l1_end && !isalpha(*(s1))) s1 ++;
    while (s2 < l2_end && !isalpha(*(s2))) s2 ++;

    if (s1 == l1_end)
    {
      if (s2 == l2_end)
        return 0;
      return 1;
    }
    if (s2 == l2_end)
    {
      return -1;
    }
    if (*s1 < *s2) return -1;
    if (*s2 < *s1) return 1;
  }

  return 0;
}

int strcompareBack(const void* a, const void* b)
{
  assert(a && b);

  const Line *l1 = (Line const *) a;
  const Line *l2 = (Line const *) b;

  char * l1_end = l1->str + l1->len;
  char * l2_end = l2->str + l2->len;

  for (char *s1 = l1_end - 1, *s2 = l2_end - 1; s1 >= l1->str && s2 >= l2->str; s1 --, s2 --)
  {

    while (s1 >= l1->str && !isalpha(*(s1))) s1 --;
    while (s2 >= l2->str && !isalpha(*(s2))) s2 --;

    if (s1 < l1->str)
    {
      if (s2 < l2->str)
        return 0;
      return 1;
    }
    if (s2 < l2->str)
    {
      return -1;
    }
    if (*s1 < *s2) return -1;
    if (*s2 < *s1) return 1;
  }

  return 0;
}



// --------------------------------------------------------------------

// -- Reading file ----------------------------------------------------

char* inputText(FILE* ftext, int size_file)
{
  assert(ftext);

  char* outtext = (char *)(calloc(size_file + 1, sizeof(*outtext)));

  if (fread(outtext, sizeof(char), size_file + 1, ftext) <= 0)
  {
    printf("%i:: Reading fail" "\n", __LINE__);
  }

  return outtext;
}

// --------------------------------------------------------------------

void writeText(FILE* fout, Line lines[], int num_elem)
{
  assert(fout);
  assert(lines);
  assert(num_elem <= MaxArr);

  char* s = NULL;
  int len = 0;
  for (int row = 0; row < num_elem; row ++)
  {
    // printf("%i:: Lines[%i/%i]..." "\n", __LINE__, row, num_rows);
      s = lines[row].str;
      len = lines[row].len;
      // printf("%i:: len = %zu" "\n", __LINE__, strlen(s));

      if (len == 1) continue;

      for (int chr = 0; chr <= len; chr ++)
      {
        if (s[chr] == '\r') continue;
        fprintf(fout, "%c", s[chr]);
      }
    // fprintf(Encyclopedia, " ::%i::%i:: S = %zu; Len = %i" "\n", __LINE__, row, lines[row].str, len);

    // fprintf(Encyclopedia, "\n");
  }
}

// -- Sorting ---------------------------------------------------------

/* 
void heapify(int ptrarr[], char* text, int positions[], int num_elem, int v)
{
    int largest = v;
    int left_child = 2 * v + 1;
    int right_child = 2 * v + 2;

    if (left_child < num_elem && compareStr(ptrarr[left_child], ptrarr[largest], positions, text))
        largest = left_child;

    if (right_child < num_elem && compareStr(ptrarr[right_child], ptrarr[largest], positions, text))
        largest = right_child;

    if (largest != v)
    {
        swap(&(ptrarr[v]), &(ptrarr[largest]));

        heapify(ptrarr, text, positions, num_elem, largest);
    }
}


void build_heap(int ptrarr[], char* text, int positions[], int num_elem)
{
  int ind = num_elem / 2 - 1;

  for (int i = ind; i >= 0; i --)
  {
    heapify(ptrarr, text, positions, i, 0);
  }

//    for (int i = 0; i < num_elem; i ++) {
//        printf("%i ", ptrarr[i]);
//        if (i == num_elem - 1) printf("\n");
//    }

}

void heap_sort(int ptrarr[], char* text, int positions[], int num_elem) {
    build_heap(ptrarr,text, positions, num_elem);

    for (int i = num_elem - 1; i >= 0; i --) {
      swap(&(ptrarr[0]), &(ptrarr[i]));

      heapify(ptrarr, text, positions, i, 0);
    }
}


 */

// --------------------------------------------------------------------
/* 

// -- QSort -----------------------------------------------------------

void quickSort(Line lines[], int left, int right, int num_elem, bool (*compare)(Line l1, Line l2))
{
  assert(left <= right);
  assert(right <= num_elem);
  assert(num_elem <= MaxArr);

  Line pivot = lines[left];
  int l_wall = left;
  int r_wall = right;

  while (left < right)
  {
    while (left < right && compare(lines[right], pivot))
    {
      right --;
    }
    if (left != right)
    {
      lines[left ++].order = lines[right].order;
      // swap(&(array[left++]), &(array[right]));
    }

    while (left < right && compare(pivot, lines[left]))
    {
      left ++;
    }
    if (left != right)
    {
      lines[right --].order = lines[left].order;
      // swap(&(array[right --]), &(array[left]));
    }
  }

  lines[left].order = pivot.order;
  pivot.order = left;
  left = l_wall;
  right = r_wall;
  if (left < pivot.order)
    quickSort(lines, left, pivot.order - 1, num_elem, compare);
  if (right > pivot.order)
    quickSort(lines, pivot.order + 1, right, num_elem, compare);
}
 */


// --------------------------------------------------------------------
