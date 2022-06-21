#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <future>
#include <cmath>
#include <python3.10/Python.h>
#include "pearson.h"
#include "processing.h"

std::vector<double> log_return(const char* f_name);
void vec_to_file(const std::vector<double>& dt, const char* f_name);

int main()
{
    auto neo = std::async(std::launch::async, log_return, "MSFT.csv");
    std::vector<double> result = neo.get();
    const char* name = "msft";
    vec_to_file(result, name);

    //Starting Python interpreter
    Py_Initialize();
    PyRun_SimpleString("import pandas as pd\n"
    "import numpy as np\n"
    "import matplotlib.pyplot as plt");

    //Building a string command to read the log_return file
    const char * py_read1 = "df = pd.read_csv('";
    const char * py_read2 = "')\n";
    char buf[1024];
    snprintf(buf, sizeof(buf), "%s%s%s", py_read1, name, py_read2);
    const char* str = buf;

    PyRun_SimpleString(str);
    //PyRun_SimpleString("df.plot(x='Date', y='Adj Close', kind='line')\n"
    PyRun_SimpleString("df.plot()\n" 
    "plt.show()\n");
    Py_Finalize();

    return 0;
}
