#include<stdio.h>
#include<algorithm>
#include<vector>
#include<queue>
#include<string>
#include<random>
using namespace std;
const vector<string> url_1 = {
    "http", "https", "ftp"
};
const vector<string> url_2 = {
    "top", "com", "xyz", "xin", "vip", "red", "net", "org", 
     "gov", "edu", "mil", "co", "biz", "name", "info", 
	"mobi", "pro", "travel", "club", "museum", "int", "aero", "post",
	"rec", "asia", "win", "cn", "fr", "us", "uk", "ru"
};
mt19937 get_rand(time(0));
const string alphabet = "0123456789abcdefghijklmnopqrstuv";
const int limitLen = (1 << 20) * 100 + 200;
char buff[limitLen];
int buffIndex;
const long long Total_len = (long long)1024 * 1024 * 1024 * 1;
inline int get_slen(){return (get_rand()&10) + 1;}
inline  int get_plen(){return (get_rand()&3) + 1;}
inline  char get_char(){return alphabet[get_rand()&31];}
inline void add_char(char tmp)
{
    buff[buffIndex++] = tmp;
}
inline void add_string()
{
    int l = get_slen();
    for(int i = 0; i < l; i++)
        add_char(get_char());
}
inline void get_url_1()
{
    for(auto ch:url_1[get_rand()&2])
        add_char(ch);
}
inline void get_url_2()
{
    for(auto ch:url_2[get_rand()&30])
        add_char(ch);
}
int main()
{
    clock_t t_st = clock();
    long long now = 0;
    freopen("../data/test.txt", "w", stdout);
    while(now <= 100000){
        long long tmp =min((long long)10000, Total_len - now);
        while(buffIndex < tmp){
            get_url_1();add_char(':'); add_char('/'); add_char('/');
            add_string();add_char('.');
            get_url_2();
            int plen = get_plen();
            for(int i = 0; i < plen; i++){
                add_char('/');
                add_string();
            }
            add_char('\n');
        }
        now += buffIndex;
        buff[buffIndex] = 0;
        printf("%s",buff);
        buffIndex = 0;
    }
    fprintf(stderr, "%.2fs\n", double(clock()-t_st)/CLOCKS_PER_SEC);
    return 0;
}
