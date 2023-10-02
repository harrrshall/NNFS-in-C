#include <stdio.h>
#include <stdlib.h>
#include <math.h>

 

typedef struct Value
{
  float val;
  float grad;
  struct Value **children;
  int n_children;
  void (*backward)(struct Value *);
} Value;

Value *make_value(float x)
{
  Value *v = (Value *)malloc(sizeof(Value));
  v->val = x;
  v->grad = 0;
  v->children = NULL;
  v->n_children = 0;
  v->backward = NULL;
  return v;
}

Value **make_values(float *arr, size_t len)
{
  Value **values = (Value **)malloc(len * sizeof(Value *));
  if (values == NULL)
  {
    perror("Memory allocation failed");
    exit(1);
  }

  for (size_t i = 0; i < len; i++)
  {
    values[i] = make_value(arr[i]);
  }
  return values;
}

void print_value(Value *v)
{
  printf("Value(val=%.2f, grad=%.2f)\n", v->val, v->grad);
}

void build_topo(Value *v, Value **topo, int *topo_size, Value **visited, int *visited_size)
{
  for (int i = 0; i < *visited_size; ++i)
  {
    if (visited[i] == v)
      return;
  }

  visited[*visited_size] = v;
  (*visited_size)++;

  for (int i = 0; i < v->n_children; ++i)
  {
    build_topo(v->children[i], topo, topo_size, visited, visited_size);
  }

  topo[*topo_size] = v;
  (*topo_size)++;
}
void grad_clip(Value *v, float min_val, float max_val)
{
  if (v->grad < min_val)
  {
    v->grad = min_val;
  }
  else if (v->grad > max_val)
  {
    v->grad = max_val;
  }
}

void add_backward(Value *v)
{
  v->children[0]->grad += v->grad;
  v->children[1]->grad += v->grad;
  grad_clip(v->children[0], -10.0, 10.0);
  grad_clip(v->children[1], -10.0, 10.0);
}

void mul_backward(Value *v)
{
  v->children[0]->grad += v->children[1]->val * v->grad;
  v->children[1]->grad += v->children[0]->val * v->grad;
  grad_clip(v->children[0], -10.0, 10.0);
  grad_clip(v->children[1], -10.0, 10.0);
}

void div_backward(Value *v)
{
  v->children[0]->grad += (1.0 / v->children[1]->val) * v->grad;
  v->children[1]->grad += (-v->children[0]->val / (v->children[1]->val * v->children[1]->val)) * v->grad;
  grad_clip(v->children[0], -10.0, 10.0);
  grad_clip(v->children[1], -10.0, 10.0);
}

void power_backward(Value *v)
{
  v->children[0]->grad += (v->children[1]->val * pow(v->children[0]->val, v->children[1]->val - 1)) * v->grad;
  if (v->children[0]->val > 0)
  {
    v->children[1]->grad += (log(v->children[0]->val) * pow(v->children[0]->val, v->children[1]->val)) * v->grad;
  }
  grad_clip(v->children[0], -10.0, 10.0);
  grad_clip(v->children[1], -10.0, 10.0);
}

void sub_backward(Value *v)
{
  v->children[0]->grad += v->grad;
  v->children[1]->grad -= v->grad;
  grad_clip(v->children[0], -10.0, 10.0);
  grad_clip(v->children[1], -10.0, 10.0);
}

Value *add(Value *a, Value *b)
{
  Value *out = (Value *)malloc(sizeof(Value));
  out->val = a->val + b->val;
  out->grad = 0;
  out->children[0] = a;
  out->children[1] = b;
  out->n_children = 2;
  out->backward = add_backward;
  return out;
}

Value *mul(Value *a, Value *b)
{
  Value *out = (Value *)malloc(sizeof(Value));
  out->val = a->val * b->val;
  out->grad = 0;
  out->children = (Value **)malloc(2 * sizeof(Value *));
  out->children = (Value **)malloc(2 * sizeof(Value *));
  out->children[0] = a;
  out->children[1] = b;
  out->n_children = 2;
  out->backward = mul_backward;
  return out;
}

Value *divide(Value *a, Value *b)
{
  if (b->val == 0.0)
  {
    printf('Error: Division by zero\n');
    exit(1);
  }
  Value *out = (Value *)malloc(sizeof(Value));
  out->val = a->val / b->val;
  out->grad = 0;
  out->children = (Value **)malloc(2 * sizeof(Value *));
  out->children[0] = a;
  out->children[1] = b;
  out->n_children = 2;
  out->backward = div_backward;
  return out;
}
Value *power(Value *a, Value *b)
{
  Value *out = (Value *)malloc(sizeof(Value));
  out->val = pow(a->val, b->val);
  out->grad = 0;
  out->children = (Value **)malloc(2 * sizeof(Value *));
  out->children[0] = a;
  out->children[1] = b;
  out->n_children = 2;
  out->backward = power_backward;
  return out;
}
Value *sub(Value *a, Value *b)
{
  Value *out = (Value *)malloc(sizeof(Value));
  out->val = pow(a->val, b->val);
  out->grad = 0;
  out->children = (Value **)malloc(2 * sizeof(Value *));
  out->children[0] = a;
  out->children[1] = b;
  out->n_children = 2;
  out->backward = sub_backward;
  return out;
}
