#include <stdio.h>
#include <stdlib.h>

#define strlen 32

typedef struct tr_node
{
  int field;
  struct tr_node *left;
  struct tr_node *right;
}trnode;
typedef struct tr_node *tlink;
typedef tlink tnode;

tnode add_el (int, tnode);
tnode del_el (int, tnode);
tnode min_el(tnode);
tnode check (int, tnode);
int str2int(char[]);
void mem_free(tnode);

tnode tcheck(tnode t, int a)
{
  if (t == NULL) return NULL;
  else if (((t->left == NULL) && (t->right == NULL)) || (t->field == a))
    return t;
  else if (t->field < a)
    return tcheck(t->right, a);
  else if (t->field > a)
    return tcheck(t->left, a);
  return NULL;
}

tnode add_el(int b, tnode t)
{
  if (t == NULL)
  {
    t = malloc(sizeof(trnode));
    t->field = b;
    t->left = NULL;
    t->right = NULL;
  }
  else  if (b < t->field)
    t->left = add_el(b, t->left);
  else if (b > t->field)
    t->right = add_el(b ,t->right);
  return t;
}

tnode min_el(tnode t)
{
  if (t->left == NULL)
    return t;
  return min_el(t->left);
}

tnode del_el(int d, tnode t)
{
  tnode tmp;
  if (t != NULL)
  {
    if (d < t->field)
      t->left = del_el(d, t->left);
    else if (d > t->field)
      t->right = del_el(d, t->right);
    else if ((t->left != NULL) && (t->right != NULL))
    {
      t->field = min_el(t->right)->field;
      t->right = del_el(t->field, t->right);
    }
    else if (t->left != NULL)
    {
      tmp = t;
      t = t->left;
      free(tmp);
    }
    else if (t->right != NULL)
    {
      tmp = t;
      t = t->right;
      free(tmp);
    }
    else if ((t->left == NULL) && (t->right == NULL))
      if (t->field == d)
      {
        t = NULL;
        free(t);
      }

    }
    return t;
}

int str2int(char strnum[])
{
  int i = 1;
  int d = 0;
  while (strnum[i] != '\0')
  {
    d = d * 10 + (strnum[i] - '0');
    i += 1;
  }
  return d;
}

void mem_clean(tnode t)
{
  if(t != NULL)
  {
    mem_clean(t->left);
    mem_clean(t->right);
    free(t);
  }
}

int main()
{
  int el;
  char c;
  char str[strlen];
  tnode tr = NULL;
  while (scanf("%s", str) != EOF)
  {
    c = str[0];
    el = str2int(str);
    if (c == '-')
    {
      tr = del_el(el, tr);
      printf ("\nElement %d was removed from the tree\n", el);
    }
    else if (c == '+')
    {
      tr = add_el(el, tr);
      printf ("\nElement %d was added to the tree\n", el);
    }
    if (c == '?')
    {
      tnode find_el = NULL;
      find_el = tcheck(tr, el);
      printf ("\nLooking for element %d.", el);
      if ((find_el == NULL) || (find_el->field != el))
        printf (" No such element. :(\n");
      else if (find_el->field == el)
        printf (" Element was found. Hurray!\n");
    }
  }
  mem_clean(tr);
  return 0;
}
