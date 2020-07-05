#include<stdio.h>

int go(int n,int p, int i){
    char a[3] = {'A','B'};
    static char st[4];
    static int ind = 0;
    
    for(i=1; i<=n; i++){
        for(j=0; j<pow(p,i); j++){
            for(k=0; k<(pow(p,n-i+1)/p); k++){
                if(n==0 && i==n ){
                    printf("%s\n",st);
                    ind = 0;
                    return;
                }
                st[ind++] = a[x];
                go(n-1,p,i+1);
            }
            if(x==n-1){
                x = 0;
                continue;
            }
            x++;
        }
    }
    
    return 0;
}

int main(){
    int n,p;
    printf("Enter the p and n: ");
    scanf("%d%d",&p,&n);
    go(n,p,1)
    /*
    for(i=1; i<=n; i++){
        for(j=0; j<pow(p,i); j++){
            for(k=0; k<(pow(p,n-i+1)/p); k++){
                st[ind++] = a[x];
            }
            if(x==n-1){
                x = 0;
                continue;
            }
            x++;
        }
    }
    */
    return 0;
}