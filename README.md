# Usage:

`./etherxtag-test HOST [OPTION...]`

| Option               | Function                                      |
|:---------------------|:----------------------------------------------|
|  -h,   --help        | Show this help and exit                       |
|  -ns,  --no-spinner  | Disable the progress spinner                  |
|  -nxt, --no-xtag     | Disable XTAG Testing                          |
|  -nxs, --no-xscope   | Disable XScope Testing                        |
|  -q,   --quiet       | Quiet mode: disable printing of mismatch data |
|  -v,   --version     | Print the version and exit                    |
|  --default-count=<n> | Send <n> packets in each of the batch tests (test specific options override this value)   |
|  --xscope-count=<n>  | Send <n> packets in the XScope batch tests    |
|  --xtag-count=<n>    | Send <n> packets in the XTAG batch tests      |


# Examples:

  Test the device at etherxtag.local quietly:
    ./etherxtag-test etherxtag.local -q

  Suitable for testing: only print test results, no spinner:
    ./etherxtag-test etherxtag.local -ns -q

  Test only the XScope portion of the device:
    ./etherxtag-test etherxtag.local -nxt

  Run a full, long test:
    ./etherxtag-test etherxtag.local --default-count=10000


# More Information:

  The program does a variety of tests on the host device. There
  are some flags to control them, as specified above. By default
  the application will print verbose output and has a run time of
  ~10s

