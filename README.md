# URL-TOP100
## **数据生成**
gen-url.cpp，随机产生总长度大于等于Total_len长度的url文件，存储在./data/文件夹下
## **算法步骤**
1.将大文件内url根据hash值，分配到若干个(此文件为4021)个小文件中
2.用map统计各个小文件内各url出现的次数，大顶堆记录小文件内部出现次数top100
3.小顶堆记录全部文件top100，与小文件各top100进行对比，若大顶堆内有大于小顶堆内的数据，进行替换。
## 测试
### 数据生成
1. 1GB文件生成 50s左右

2. 10GB文件生成 500s左右

### 算法
1. 小规模数据测试算法正确性

2. 1GB文件测试  耗费时间170s左右

3. 10GB文件测试 1021s + 1301s

4. 100GB文件测试  空间不足，未能测试