#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct Vertex {
    int value;
    int from;
    int to;
    struct Vertex* l;
    struct Vertex* r;
    long long odd_sum;
    long long even_sum;
    int odd_cnt;
    int even_cnt;
    int incr_cnt;
};

void calc_add_info(struct Vertex* v) {
    if (v == NULL) {
        return;
    }
    if (v->value == -1) {
        v->odd_sum = v->l->odd_sum + v->r->odd_sum;
        v->odd_cnt = v->l->odd_cnt + v->r->odd_cnt;
        v->even_sum = v->l->even_sum + v->r->even_sum;
        v->even_cnt = v->l->even_cnt + v->r->even_cnt;
    } else {
        bool is_even = v->value % 2 == 0;
        v->odd_sum = is_even ? 0 : v->value;
        v->even_sum = is_even ? v->value : 0;
        v->odd_cnt = is_even ? 0 : 1;
        v->even_cnt = is_even ? 1 : 0;
    }
    if (v->incr_cnt > 0) {
        v->odd_sum += v->odd_cnt * v->incr_cnt;
        v->even_sum += v->even_cnt * v->incr_cnt;

        if (v->incr_cnt % 2 == 1) {
            v->odd_sum = v->odd_sum ^ v->even_sum;
            v->even_sum = v->odd_sum ^ v->even_sum;
            v->odd_sum = v->odd_sum ^ v->even_sum;

            v->odd_cnt = v->odd_cnt ^ v->even_cnt;
            v->even_cnt = v->odd_cnt ^ v->even_cnt;
            v->odd_cnt = v->odd_cnt ^ v->even_cnt;
        }
    }
}

void init(struct Vertex* v, int* a, int n, int from) {
    if (v == NULL) {
        return;
    }
    if (n > 1) {
        int n_l = n / 2 + (n % 2 == 0 ? 0 : 1);
        int n_r = n / 2;

        v->value = -1;
        v->from = from;
        v->to = from + n - 1;
        v->l = (struct Vertex*)malloc(sizeof(struct Vertex));
        init(v->l, a, n_l, from);
        v->r = (struct Vertex*)malloc(sizeof(struct Vertex));
        init(v->r, a, n_r, n_l + from);
        v->incr_cnt = 0;
    } else {
        v->value = a[from];
        v->from = from;
        v->to = from;
        v->l = NULL;
        v->r = NULL;
        v->incr_cnt = -1;
    }
    calc_add_info(v);
}

void incr_incr(struct Vertex* v, int incr) {
    if (v->incr_cnt == -1) {
        v->value += incr;
    } else {
        v->incr_cnt += incr;
    }
}

void check_if_incr(struct Vertex* v) {
    if (v == NULL) {
        return;
    }
    if (v->incr_cnt > 0 && v->value == -1) {
        incr_incr(v->l, v->incr_cnt);
        calc_add_info(v->l);
        incr_incr(v->r, v->incr_cnt);
        calc_add_info(v->r);
        v->incr_cnt = 0;
    }
}

void inc(struct Vertex* v, int l, int r, int c) {
    if (v == NULL) {
        return;
    }
    if (v->from > l) l = v->from;
    if (v->to < r) r = v->to;
    if (l > r) return;

    check_if_incr(v);

    if (v->from == l && v->to == r) {
        if (v->incr_cnt == -1) {
            v->value += c;
        } else {
            v->incr_cnt += c;
        }
    } else {
        inc(v->l, l, r, c + v->incr_cnt);
        inc(v->r, l, r, c + v->incr_cnt);
        v->incr_cnt = 0;
    }
    calc_add_info(v);
}

void set(struct Vertex* v, int pos, int value) {
    if (v == NULL) {
        return;
    }
    if (v->from > pos || pos > v->to) {
        return;
    }

    check_if_incr(v);

    if (v->from == pos && v->to == pos) {
        v->value = value;
    } else {
        set(v->l, pos, value);
        set(v->r, pos, value);
    }
    calc_add_info(v);
}

long long getSumEven(struct Vertex* v, int l, int r) {
    if (v == NULL) {
        return 0;
    }
    if (v->from > l) l = v->from;
    if (v->to < r) r = v->to;
    if (l > r) return 0;

    if (v->from == l && v->to == r) {
        return v->even_sum;
    }
    check_if_incr(v);
    return getSumEven(v->l, l, r) + getSumEven(v->r, l, r);
}

long long getSumOdd(struct Vertex* v, int l, int r) {
    if (v == NULL) {
        return 0;
    }
    if (v->from > l) l = v->from;
    if (v->to < r) r = v->to;
    if (l > r) return 0;

    if (v->from == l && v->to == r) {
        return v->odd_sum;
    }
    check_if_incr(v);
    return getSumOdd(v->l, l, r) + getSumOdd(v->r, l, r);
}

void clean(struct Vertex* v) {
    if (v->l != NULL) {
        clean(v->l);
    }
    if (v->r != NULL) {
        clean(v->r);
    }
    free(v);
}

int main() {
    int n, q;
    scanf("%d %d", &n, &q);

    int* a = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }

    struct Vertex* root = (struct Vertex*)malloc(sizeof(struct Vertex));
    init(root, a, n, 0);

    int cmd_buffer;
    int param1;
    int param2;
    for (int i = 0; i < q; ++i) {
        scanf("%d", &cmd_buffer);
        scanf("%d %d", &param1, &param2);
        param1--;
        switch (cmd_buffer) {
            case 1:
                set(root, param1, param2);
                break;
            case 2:
                param2--;
                inc(root, param1, param2, 1);
                break;
            case 3:
                param2--;
                printf("%lld\n", getSumEven(root, param1, param2));
                break;
            case 4:
                param2--;
                printf("%lld\n", getSumOdd(root, param1, param2));
                break;
        }
    }

    free(a);
    clean(root);
    return 0;
}