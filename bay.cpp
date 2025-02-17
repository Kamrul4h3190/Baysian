#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<vector>
#include<iostream>
#include<bits/stdc++.h>
using namespace std ;

vector<string>avalue ;
vector<string>cvalue ;

int numberofobject = 0 ;
int totaldog = 0;
int totalcat = 0;

int findOccurance(string att, string cl)
{
    FILE *fptr;
    char a[100], c[100] ;
    int count = 0 ;
    if ((fptr = fopen("data.txt", "r")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    while (fscanf(fptr, "%s %s", a, c)!= EOF){
        string s = "" ;
        string s2 = "" ;

        for(int i = 0 ; i < strlen(a) ; i++){
            s += a[i] ;
         }
         for(int i = 0 ; i < strlen(c) ; i++){
            s2 += c[i] ;
         }
         if(s == att && s2 == cl) count++ ;
    }
    fclose(fptr);

    return count ;
}

void trainModel(){
    char a[100], c[100];
    FILE *fptr;
    if ((fptr = fopen("data.txt", "r")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    string s = "" ;
    while (fscanf(fptr, "%s %s", a, c)!= EOF){
         printf("%s %s", a, c);
         printf("\n") ;
         numberofobject++ ;

         bool flag = false ;
         string s = "" ;
         for(int i = 0 ; i < strlen(a) ; i++){
            s += a[i] ;
         }
         for(int i = 0 ; i < avalue.size() ; i++){
            if(s == avalue[i]) {
                flag = true ;

            }
         }
         if(!flag)
            avalue.push_back(s) ;


         flag = false ;
         s = "" ;
         for(int i = 0 ; i < strlen(c) ; i++){
            s += c[i] ;
         }
         for(int i = 0 ; i < cvalue.size() ; i++){
            if(s == cvalue[i]) {
                flag = true ;
                //editing here
                if(s.compare("dog")){
                    totaldog++;
                }else if(s.compare("cat")){
                    totalcat++;
                }
            }
         }

         if(!flag){
            cvalue.push_back(s) ;
            if(s.compare("dog")){
                    totaldog++;
                }else if(s.compare("cat")){
                    totalcat++;
                }
         }

    }
    printf("total sample = %d\n", numberofobject) ;
    for(int i = 0 ; i < avalue.size() ; i++){

        cout<< avalue[i] << endl ;
    }

    //find and write priors to prior.txt
    for(int i = 0 ; i < cvalue.size() ; i++){

        if(cvalue[i].compare("dog")){
            fptr = fopen("prior.txt", "a");
            fprintf(fptr, "%s = %.2f\n",cvalue[i].c_str(),totaldog/(1.00*numberofobject) );
            fclose(fptr);

            cout<< cvalue[i] <<" = "<<totaldog<<"|"<<numberofobject<<endl ;
        }
        else if(cvalue[i].compare("cat")){
            fptr = fopen("prior.txt", "a");
            fprintf(fptr, "%s = %.2f\n",cvalue[i].c_str(),totalcat/(1.00*numberofobject) );
            fclose(fptr);
            cout<< cvalue[i] <<" = "<<totalcat<<"|"<<numberofobject<<endl ;
        }

    }

    for(int i = 0 ; i < cvalue.size() ; i++){
        for(int j = 0 ; j < avalue.size() ; j++){
            int n = findOccurance(avalue[j], cvalue[i]) ;

            fptr = fopen("likelyhood.txt", "a");
            fprintf(fptr, "%s|%s = %d\n",avalue[j].c_str(),cvalue[i].c_str(),n);
            fclose(fptr);
            cout << avalue[j] << "|" << cvalue[i] << " = "<< n << endl ;

        }
    }

    fclose(fptr);
}

double extractPrior(char* cl){
    FILE *fptr;
    double prior = 0.0;

    if ((fptr = fopen("prior.txt", "r")) == NULL) {
        printf("Error! opening prior.txt");
        exit(1);
    }
    char line[100];
    while(fgets(line,sizeof(line),fptr)){
        //checking cl(class) exist in line extracted from prior.txt
        if( strstr(line,cl)!=NULL ){
            int index;
            char *ch;
            ch = strchr(line,'=');
            index = (int)(ch - line);

            string sub = "";

            for(int i = index+2; i< sizeof(line); i++){
                sub+= line[i];
            }

            prior = atof(sub.c_str());
            fclose(fptr);
        }
    }
    //printf("prior is %f\n",prior);
    return prior;
}

double extractLikelyhood(char* description){
    FILE *fptr;
    double lkhood = 0.0;

    if ((fptr = fopen("likelyhood.txt", "r")) == NULL) {
        printf("Error! opening prior.txt");
        exit(1);
    }
    char line[100];
    while(fgets(line,sizeof(line),fptr)){
        //checking cl(class) exist in line extracted from prior.txt
        if( strstr(line,description)!=NULL ){
            int index;
            char *ch;
            ch = strchr(line,'=');
            index = (int)(ch - line);

            string sub = "";

            for(int i = index+2; i< sizeof(line); i++){
                sub+= line[i];
            }

            lkhood = atof(sub.c_str());
            fclose(fptr);
        }
    }
    printf("likelyhood is %f\n",lkhood);
    return lkhood;
}

void testModel(string attribute){
    double pro_cat = 2.0;
    double pro_dog = 0.0;

    double pri_cat = extractPrior("cat");
    double pri_dog = extractPrior("dog");

    double lik_cat = extractLikelyhood("small|cat");
    double lik_dog = extractLikelyhood("small|dog");

    if(pro_cat>pro_dog){
        printf("Sample is cat\n");
    }else if(pro_cat<pro_dog){
        printf("Sample is dog\n");
    }
}
int main() {
    //trainModel();
    string attribute = "small";
    testModel(attribute);
    extractPrior("cat");
    return 0;
}

