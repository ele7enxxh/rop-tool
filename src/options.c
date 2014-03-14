#include "ropc.h"

/* Options */
char options_filename[PATH_MAX]       = "./a.out";
enum MODE options_mode                = MODE_GADGET;
enum FLAVOR options_flavor            = FLAVOR_INTEL;
enum OUTPUT options_output            = OUTPUT_PERL;
int options_color                     = 1;
int options_raw                       = 0;
uint8_t options_depth                 = 15;
int options_filter                    = 1;
BLIST options_bad                     = {NULL, 0};
BLIST options_search                  = {NULL, 0};

/* Display program version & quit */
static void version(void) {
  printf("%s version %s\n", PACKAGE, VERSION);
  printf("Compiled the %s at %s\n", __DATE__, __TIME__);
  exit(EXIT_SUCCESS);
}

/* Display program usage & quit */
static void usage(const char *progname) {
  printf("Usage : %s [OPTIONS] filename\n", progname);
  printf("Tool for searching Gadgets in ELF binaries\n");
  printf("\n");
  printf("MODES\n");
  printf("  -G --gadget        Gadget searching mode\n");
  printf("  -S --string        String searching mode (argument required)\n");
  printf("  -P --payload       Payload generator mode\n");
  printf("\n");
  printf("Filter options\n");
  printf("  -b, --bad          Specify bad chars\n");
  printf("  -d, --depth        Specify the depth searching (gadget mode only)\n");
  printf("  -a, --all          Display all gadgets (gadget mode only)\n");
  printf("\n");
  printf("Output options\n");
  printf("  -n, --no-color     No colors\n");
  printf("  -f, --flavor       Specify the flavor (gadget mode only) : intel or att\n");
  printf("\n");
  printf("General options\n");
  printf("  -r, --raw          Open file in raw mode\n");
  printf("  -h, --help         Print help\n");
  printf("  -v, --version      Print version\n");
  exit(EXIT_SUCCESS);
}

enum FLAVOR options_set_flavor(const char *flavor) {
  if(!strcmp(flavor, "intel"))
    return FLAVOR_INTEL;
  if(!strcmp(flavor, "att"))
    return FLAVOR_ATT;

  FATAL_ERROR("%s: bad flavor", flavor);

  return FLAVOR_NONE;
}

void options_parse(int argc, char **argv) {
  int opt;
  char *progname = argv[0];
  const struct option opts[] = {
    {"gadget",      no_argument,       NULL, 'G'},
    {"string",      required_argument, NULL, 'S'},
    {"flavor",      required_argument, NULL, 'f'},
    {"bad",         required_argument, NULL, 'b'},
    {"depth",       required_argument, NULL, 'd'},
    {"all",         no_argument,       NULL, 'a'},
    {"help",        no_argument,       NULL, 'h'},
    {"no-color",    no_argument,       NULL, 'n'},
    {"raw",         no_argument,       NULL, 'r'},
    {"version",     no_argument,       NULL, 'v'},
    {NULL,          0,                 NULL, 0  }
  };

  while((opt = getopt_long(argc, argv, "GS:f:b:d:ahnvr", opts, NULL)) != -1) {
    switch(opt) {

    case 'G':
      options_mode = MODE_GADGET;
      break;

    case 'S':
      options_mode = MODE_STRING;
      options_search = opcodes_to_blist(optarg);
      break;

    case 'f':
      options_flavor = options_set_flavor(optarg);
      break;

    case 'b':
      options_bad = opcodes_to_blist(optarg);
      break;

    case 'd':
      options_depth = atoi(optarg);
      break;

    case 'a':
      options_filter = 0;
      break;

    case 'h':
      usage(progname);
      break;

    case 'n':
      options_color = 0;
      break;

    case 'v':
      version();
      break;

    case 'r':
      options_raw = 1;
      break;

    default:
      usage(progname);      
    }
  }

  if(options_depth > MAX_DEPTH)
    FATAL_ERROR("Depth must be in range 0-%d", MAX_DEPTH);

  if(optind < argc) {
    strncpy(options_filename, argv[optind], PATH_MAX-1);
    options_filename[PATH_MAX-1] = '\0';
  }
}