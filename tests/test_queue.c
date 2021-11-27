#include "munit.h"
#include "queue.h"
#include "mandel-tiles-graphic.h"

#include <stdbool.h>

static MunitResult test_init(const MunitParameter params[], void* data) {
    Queue myQueue;
    return MUNIT_OK;
}

static MunitResult test_queue(const MunitParameter params[], void* data) {
    Queue* myQueue = (Queue*) malloc(sizeof(Queue));
    fractal_param_t* item = (fractal_param_t*) malloc(sizeof(fractal_param_t));
    item->left = 240; item->low = 240;
    item->ires = 80; item->jres = 60;
    item->xmin = 0.2709203750; item->ymin = 0.0047495000;
    item->xmax = 0.2709205000; item->ymax = 0.0047496250;
    enqueue(myQueue, item);
    return MUNIT_OK;
}

/*
Compares two fractal parameters, returns true if they are equal (their 
attributes are equal), false otherwise
*/
bool fractal_param_compare(fractal_param_t* p1, fractal_param_t* p2){
    fractal_param_t param1 = *p1;
    fractal_param_t param2 = *p2;

    if (param1.low != param2.low){
        return false;
    }
}

static MunitResult test_queue(const MunitParameter params[], void* data) {
    Queue* myQueue = (Queue*) malloc(sizeof(Queue));
    fractal_param_t* item = (fractal_param_t*) malloc(sizeof(fractal_param_t));
    item->left = 240; item->low = 240;
    item->ires = 80; item->jres = 60;
    item->xmin = 0.2709203750; item->ymin = 0.0047495000;
    item->xmax = 0.2709205000; item->ymax = 0.0047496250;
    enqueue(myQueue, item);
    return MUNIT_OK;
}

static MunitResult test_compare(const MunitParameter params[], void* data) {
  /* We'll use these later */
  const unsigned char val_uchar = 'b';
  const short val_short = 1729;
  double pi = 3.141592654;
  char* stewardesses = "stewardesses";
  char* most_fun_word_to_type;

  /* These are just to silence compiler warnings about the parameters
   * being unused. */
  (void) params;
  (void) data;

  /* Let's start with the basics. */
  munit_assert(0 != 1);
  munit_assert_false(0);
  munit_assert_char('a', ==, 'a');
  munit_assert_uchar(val_uchar, ==, 'b');

  return MUNIT_OK;
}

/*  ===============================
            Test Suite
    ===============================
*/
static MunitTest test_suite_tests[] = {
  { (char*) "/queue/init", test_init, NULL, NULL, 0, NULL },

  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

/* If you wanted to have your test suite run other test suites you
 * could declare an array of them.  Of course each sub-suite can
 * contain more suites, etc. */
/* static const MunitSuite other_suites[] = { */
/*   { "/second", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE }, */
/*   { NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE } */
/* }; */

static const MunitSuite test_suite = 
{ (char*) "", test_suite_tests, NULL, 1, 0};

#include <stdlib.h>

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  /* Finally, we'll actually run our test suite!  That second argument
   * is the user_data parameter which will be passed either to the
   * test or (if provided) the fixture setup function. */
  return munit_suite_main(&test_suite, (void*) "µnit", argc, argv);
}