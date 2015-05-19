/* 
 * Copyright (c) 2015 Alexandre Clément
 * seqBang is released under MIT license.
 * For more information view License.txt
 * or visit http://opensource.org/licenses/MIT
 * 
 * alexandre.r.clement@gmail.com
 */
#include "m_pd.h"

static t_class *seqBang_class;

typedef struct _seqBang {
	t_object x_obj;

	t_float numSlots;
	t_float activeSlot;
	t_outlet ** slots;

}t_seqBang;

void seqBang_message(t_seqBang *x)
{
	int num_outlets = (int) x->numSlots - 1;

	x->activeSlot = x->activeSlot < 0 ? 0 : x->activeSlot;
	x->activeSlot = x->activeSlot > num_outlets ? 0 : x->activeSlot;

	outlet_bang(x->slots[(int)x->activeSlot]);

	x->activeSlot++;
}

void *seqBang_new(t_floatarg count_arg) {
	t_seqBang *x = (t_seqBang *)pd_new(seqBang_class);

	int i;

	x->numSlots = count_arg <= 2 ? 2 : count_arg;
	x->numSlots = x->numSlots > 40 ? 40 : x->numSlots;

	x->slots = getbytes(x->numSlots * sizeof(t_outlet*));

	for (i=0; i< (int) x->numSlots; i++) {
		x->slots[i] = outlet_new(&x->x_obj, gensym("float"));
	}

	x->activeSlot = 0;

	floatinlet_new(&x->x_obj, &x->activeSlot);


	return (void *) x;
}

void seqBang_die(t_seqBang *x){
	int i;
	for (i=0; i< (int) x->numSlots; i++) {
		outlet_free(x->slots[i]);
	}
}

void seqBang_setup(void) {
	seqBang_class = class_new(gensym("seqBang"),
			(t_newmethod)seqBang_new,
			(t_method)seqBang_die,
			sizeof(t_seqBang),
			CLASS_DEFAULT,
			A_DEFFLOAT,
			0);

	class_addbang(seqBang_class, seqBang_message);
}
