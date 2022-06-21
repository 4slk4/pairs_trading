#ifndef PROCESSING_H
#define PROCESSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>

void vec_to_file(const std::vector<double>& dt, const char* f_name){
    FILE* fp = fopen(f_name, "w");
    if (!fp){
        printf("Cant create the file!\n");
        exit(1);
    }

    for (auto i : dt){
        fprintf(fp, "%f\n", i);
    }

    fclose(fp);
}


std::vector<double> log_return(const char* f_name){
    std::vector<double> adj_return;
    std::vector<double> s_return;
    
    //Read the file
    FILE* fp = fopen(f_name, "r");

    if (!fp){
        printf("Can't open file %s\n", f_name);
        exit(1);
    }
    else{
        char buffer[1024];

        int row = 0;
        int column = 0;

        while (fgets(buffer, 1024, fp)){
            column = 0;
            row++;
            
            if (row == 1)
                continue;
            
            char* value = strtok(buffer, ", ");

            while (value){
                if (column == 5){
                    float num = atof(value);
                    adj_return.push_back(num);
                }
                value = strtok(NULL, ", ");
                column++;
            }
        }

        fclose(fp);
    }

    //Calculate Log Return
    for (int i = 1; i < adj_return.size(); i++){
        double log_return = log(adj_return[i]/adj_return[i-1]);
        s_return.push_back(log_return);
    }

    return s_return;
}

#endif