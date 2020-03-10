#include<stdio.h>
#include<algorithm>
#include<vector>
#include<queue>
#include<string>
#include<random>
#include<iostream>
#include<fstream>
#include<unordered_set>
#include<unordered_map>
using namespace std;
typedef unsigned long long ull;
const string data_path = "../data/test.txt";
const string block_path = "../tmp/";
const unsigned radix = 33, blocks = 4021, limit = 100;

//哈希函数
unsigned Hash(const string &url) 
{
	unsigned res = 0;
	for(char ch:url)
		res = (res * radix + unsigned(ch)) % blocks;
	return res;
}
//将大文件分类到小文件中
void classify_to_blocks(vector<ofstream> &ofs)
{
	ifstream fin(data_path);
	if(fin.fail())
	{
		printf("data_path error: %s\n",data_path.c_str());
		exit(1);
	}

	string url;
	vector<ull> block_count(blocks);
	while(getline(fin, url)) 
		if(url!=""){
			unsigned hash_val = Hash(url);
			ofs[hash_val] << url << "\n";
			++block_count[hash_val];
		}
	fin.close();
	for(int block_id = 0; block_id < blocks; block_id++)
		ofs[block_id].close();

	printf("classify end.\n");
	printf("total urls: %llu\n", accumulate(block_count.begin(), block_count.end(), 0ull) );
	printf("ave urls of block: %llu\n", accumulate(block_count.begin(), block_count.end(), 0ull)/blocks );
	printf("max urls of block: %llu\n", *max_element(block_count.begin(), block_count.end()));
}
//逐个找到每个文件的top100，并且与tatal进行比较替换
vector< pair<string, unsigned>> find_top_from_blocks()
{
	using psu = pair<string, unsigned>;
	auto big_top = [](psu &a, psu &b){return a.second < b.second;};  //小文件大顶堆
	auto little_top = [](psu &a, psu &b){return a.second > b.second;}; // 全部文件小顶堆

	vector<psu> total_heap(limit, make_pair("",0)); 

	vector<ull> block_count(blocks);
	for(unsigned block_id=0; block_id<blocks; block_id++)
	{
		ifstream fin(block_path + to_string(block_id));
		if(fin.fail())
		{
			printf("block_path error: %s\n",(block_path + to_string(block_id)).c_str());
			exit(1);
		}

		string url;
		unordered_map<string, unsigned> counter;

		while(getline(fin, url))
			counter[url]++;

		block_count[block_id] = counter.size();
		printf("block_id=%u classes=%u\n",block_id,(unsigned)counter.size() );

		vector<psu> heap(counter.begin(), counter.end()); 
		make_heap(heap.begin(), heap.end(), big_top);

		for(unsigned i=0, lim = min(limit, (unsigned)heap.size()); i<lim; ++i)
		{
			if(heap.front().second <= total_heap.front().second) 
				break;

			//tatal替换最小元素
			pop_heap(total_heap.begin(), total_heap.end(), little_top); // 将最小元素移到最后
			total_heap.back() = heap.front(); // 更新最后一个元素
			push_heap(total_heap.begin(), total_heap.end(), little_top); // 更新total_heap

			//heap删除最小元素
			pop_heap(heap.begin(), heap.end(), big_top); 
			heap.pop_back(); 
		}
		fin.close();
	}
	
	sort(total_heap.begin(), total_heap.end(), little_top); 

	return total_heap;
}
//初始化tmp 并返回文件流
vector<ofstream> init()
{
	string command = "mkdir -p " + block_path;
	if(system(command.c_str())!=0)
	{
		printf("make dir error: %s\n", command.c_str());
	}
	vector<ofstream> ofs(blocks);
	for(unsigned block_id=0; block_id<blocks; ++block_id)
	{
		string block_id_path = block_path + to_string(block_id);
		ofs[block_id].open(block_id_path);
		if(ofs[block_id].fail())
		{
			printf("block_path error: %s\n",block_id_path.c_str() );
			exit(1);
		}
	}
	return ofs;
}
int main(void)
{
	clock_t t_st = clock(); 
	//vector<ofstream> ofs = init();
	//printf("init time cost = %.2fs\n\n",double(clock()-t_st)/CLOCKS_PER_SEC );

	 //t_st = clock();
	 //printf("blocks = %d, hash radix = %d, find top %d\n\n", blocks, radix, limit );

	 //classify_to_blocks(ofs);
	 //printf("classify_to_blockstime cost = %.2fs\n\n",double(clock()-t_st)/CLOCKS_PER_SEC );
     //t_st = clock();

	 std::vector<std::pair<string, unsigned>> top = find_top_from_blocks();
	 printf("find_top_from_blocks time cost = %.2fs\n\n",double(clock()-t_st)/CLOCKS_PER_SEC );

	 printf("done.\n");
	 for(auto psu:top)
	 	printf("%d %s\n", psu.second, psu.first.c_str() );
	 printf("\n");

    return 0;
}