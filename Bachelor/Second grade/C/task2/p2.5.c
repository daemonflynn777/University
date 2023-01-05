#include <stdio.h>
#include <stdlib.h>
#define n 30

typedef struct node *link;
typedef struct node
{
  struct node *next;
  char *str;
}node;
typedef link lst;

lst wrd_add(char *w, link l)
{
  if (l == NULL)
  {
    l = malloc(sizeof(node));
    l->str = malloc(30*sizeof(char));
    l->str = w;
    l->next = NULL;
  }
  else
    l->next = wrd_add(w, l->next);
  return l;
}

int wrd_cmp(char *w1, char *w2)
{
  while ((*w1 != '\0') || (*w1 != '\0'))
  {
    if (*w1 != *w2)
      return 0;
    w1++;
    w2++;
  }
  if ((*w1 != '\0') || (*w2 != '\0'))
    return 0;
  return 1;
}

lst wrd_del(lst l, char *wr)
{
  lst del_el;
  char *tmp_del;
  if (l->next != NULL)
  {
    if (wrd_cmp(l->str, wr) == 1)
    {
      del_el = l;
      tmp_del = l->str;
      l = l->next;
      free(del_el);
      free(tmp_del);
      l = wrd_del(l, wr);
    }
    else
      l->next = wrd_del(l->next, wr);
    }
    return l;
}

void lst_pr(lst l)
{
  if (l != NULL)
  {
    char *tmpstr;
    tmpstr = l->str;
    while (*tmpstr != '\0')
    {
      printf("%c", *tmpstr);
      ++tmpstr;
    }
    printf(" ");
    lst_pr(l->next);
  }
}

void lst_del(lst l)
{
  lst tmp_l;
  char *dlw;
  while (l != NULL)
  {
    tmp_l = l;
    dlw = l->str;
    l = l->next;
    free(tmp_l);
    free(dlw);
  }
}

int main()
{
  char lastwrd[n];
  char *wrd;
  lst ls = NULL;
  lst tmp;
  int count = 0;

  printf("Enter your text:\n");
  wrd = malloc(30*sizeof(char));
  while (scanf("%s", wrd) != EOF)
    {
      ls = wrd_add(wrd, ls);
      count += 1;
      wrd = malloc(30*sizeof(char));

    }
  if (count == 0)
  {
    printf("The list is empty. Program ended.");
    return 0;
  }
  tmp = ls;
  while (tmp->next != NULL)
    tmp = tmp->next;
  int i = 0;
  while (tmp->str[i] != '\0')
  {
    lastwrd[i] = tmp->str[i];
    i++;
  }
  lastwrd[i] = '\0';
  ls = wrd_del(ls, lastwrd);
  printf("The result is:\n");
  lst_pr(ls);
  printf("\n");
  lst_del(ls);
}
