#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Class implementation #1 */

void setI(struct foo* fooPtr, int i);
int getI(struct foo* fooPtr);

struct privateFields {

	int m_i;
	float m_j;
};

struct foo {
	
	struct privateFields* privFields;

	void (*m_setI)(struct foo*, int i);
	int (*m_getI)(struct foo*);

};

/* Constructor */
void foo_construct(struct foo* fooPtr, const int i, const int j) {

	puts("Construct #1");

	/* Init of privates */
	fooPtr->privFields = (struct privateFields*)malloc(sizeof(struct privateFields));
	assert(fooPtr->privFields);

	fooPtr->privFields->m_i = i;
	fooPtr->privFields->m_j = j;

	/* Init of methods */
	fooPtr->m_setI = setI;
	fooPtr->m_getI = getI;
}

/* Destructor */
void foo_destruct(struct foo* fooPtr) {

	puts("Destruct #1");
	free(fooPtr->privFields);
}


/* Setters/getters */
void setI(struct foo* fooPtr, int i) {

	fooPtr->privFields->m_i = i;
}

int getI(struct foo* fooPtr) {

	return fooPtr->privFields->m_i;
}

/* Class implementation #2  */

typedef struct privateMonkey {

	int m_age;
	int m_jumpHight;
} privateMonkey;

struct monkey;

typedef struct monkey {

    struct privateMonkey* privFlds;
    void (*m_showAge)(struct monkey*);
    void (*m_doJump)(struct monkey*);

} monkey;

void showAge(struct monkey* this_) {

	printf("The age = %d\n", this_->privFlds->m_age);
}

void doJump(struct monkey* this_) {

	printf("Jump at = %d meters hight\n", this_->privFlds->m_jumpHight);
}

monkey* monkey_construct(int age, int jumpHight) {

	puts("Construct #2");

	monkey* this_ = (monkey*)malloc(sizeof(struct monkey));
	privateMonkey* privImpl_ = (privateMonkey*)malloc(sizeof(struct privateMonkey));

	/* Init fields */

	this_->privFlds = privImpl_;
	this_->privFlds->m_age = age;
	this_->privFlds->m_jumpHight = jumpHight;

	/* Init methods */
	this_->m_showAge = showAge;
	this_->m_doJump = doJump;

	return this_;
}

void monkey_destruct(monkey* this_) {

	puts("Destruct #2");

	free(this_->privFlds);
	free(this_);
}


/***********************************************/

int main() {

	/* Impl #1 */
	puts("Impl #1 ====================");

	foo f;
	foo_construct(&f, 42, 777);

	printf("Just a getter of I-field (before) -> %d\n", f.m_getI(&f));
	
	puts("Setting new value...");
	f.m_setI(&f, 88);
	
	printf("Just a getter of I-field (after) -> %d\n", f.m_getI(&f));

	foo_destruct(&f);

	/* Impl #2 */
	puts("\nImpl #2 ====================");

	monkey* myMonkey = monkey_construct(18, 2);

	myMonkey->m_showAge(myMonkey);
	myMonkey->m_doJump(myMonkey);

	monkey_destruct(myMonkey);
	return 0;
}