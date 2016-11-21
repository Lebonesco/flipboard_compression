#include <Python.h>
#include <stdio.h>
#include <string.h>

typedef struct node_t {
  struct node_t *left, *right;
  int freq;
  char c;
} *node;
 
struct node_t pool[256] = {{0}};
node qqq[255], *q = qqq - 1;
int n_nodes = 0, qend = 1;
char *code[128] = {0}, buf[1024];
 
node new_node(int freq, char c, node a, node b)
{
  node n = pool + n_nodes++;
  if (freq) n->c = c, n->freq = freq;
  else {
    n->left = a, n->right = b;
    n->freq = a->freq + b->freq;
  }
  return n;
}
 
/* priority queue */
void qinsert(node n)
{
  int j, i = qend++;
  while ((j = i / 2)) {
    if (q[j]->freq <= n->freq) break;
    q[i] = q[j], i = j;
  }
  q[i] = n;
}
 
node qremove()
{
  int i, l;
  node n = q[i = 1];
 
  if (qend < 2) return 0;
  qend--;
  while ((l = i * 2) < qend) {
    if (l + 1 < qend && q[l + 1]->freq < q[l]->freq) l++;
    q[i] = q[l], i = l;
  }
  q[i] = q[qend];
  return n;
}
 
/* walk the tree and put 0s and 1s */
void build_code(node n, char *s, int len)
{
  static char *out = buf;
  if (n->c) {
    s[len] = 0;
    strcpy(out, s);
    code[n->c] = out;
    out += len + 1;
    return;
  }
 
  s[len] = '0'; build_code(n->left,  s, len + 1);
  s[len] = '1'; build_code(n->right, s, len + 1);
}
 
void init(const char *s)
{
  int i, freq[128] = {0};
  char c[16];
 
  while (*s) freq[(int)*s++]++;
 
  for (i = 0; i < 128; i++)
    if (freq[i]) qinsert(new_node(freq[i], i, 0, 0)); // if character in string, create node that holds the character, character frequency and right and left reference. insert into priority queue
 
  while (qend > 2) 
    qinsert(new_node(0, 0, qremove(), qremove()));
 
  build_code(q[1], c, 0);
}
 
void encode(const char *s, char *out)
{
  while (*s) {
    strcpy(out, code[*s]);
    out += strlen(code[*s++]);
  }
}
 
void decode(const char *s, node t)
{
  node n = t;
  while (*s) {
    if (*s++ == '0') n = n->left;
    else n = n->right;
 
    if (n->c) putchar(n->c), n = t;
  }
 
  putchar('\n');
  if (t != n) printf("garbage input\n");
}

void printBinary(int n) {
  while (n) {
    if (n & 1)
        printf("1");
    else
        printf("0");

    n >>= 1;
}
}
char * compress(char *str)
{
  int i;
  // const char *str = "this is an example for huffman encoding"; 
        char buf[1024];
 
  init(str); // calculates frequency and builds huffman tree
  for (i = 0; i < 128; i++)
    // if (code[i]) printf("'%c': %s\n", i, code[i]);
 
  encode(str, buf); // this is where python argurments will get passed in.
  // printf("encoded: %s\n", buf);

  int j;
  // printf("original bits: ");
  for(j =0; j<strlen(str); j++) {
    // printBinary(str[j]);
  }
  // printf("\n");

  // printf("decoded: ");
  decode(buf, q[1]);


  char *returnString = malloc(sizeof(char) * strlen(buf));
  int k;
  for(k = 0; k < strlen(buf); ++k) {
        returnString[k] = buf[k];
    }
    // printf("return String: %s\n", returnString);
    return returnString;
  return 0;
}

static PyObject* encoding(PyObject* self, PyObject* args) {
	char* n; // incoming python value

	if(!PyArg_ParseTuple(args, "s", &n)) { //placing arg integer into n, otherwise return NULL
		return NULL;
	}

	return Py_BuildValue("s", compress(n)); // converts c value back to python

}

static PyObject* version(PyObject* self) {
	return Py_BuildValue("s", "Version 1.0");
}

static PyMethodDef myMethods[] = {
	{"encoding", encoding, METH_VARARGS, "encode text"},
	{"version", (PyCFunction)version, METH_NOARGS, "returns the version."},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef myModule = {
	PyModuleDef_HEAD_INIT, 
	"myModule", 
	"Compression Module",
	-1,
	myMethods
};

PyMODINIT_FUNC PyInit_myModule(void) {
	return PyModule_Create(&myModule);
}