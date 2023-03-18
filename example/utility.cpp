// #include <bluseio/utility/biolog.h>
#include <bluseio/utility/block_queue.h>
#include <bluseio/utility/bloom_filter.h>
#include <bluseio/utility/circular_buffer.h>
#include <bluseio/utility/coredump.h>
#include <bluseio/utility/crc.h>
#include <bluseio/utility/iso8601.h>
#include <bluseio/utility/list.h>
#include <bluseio/utility/min_top_heap.h>
#include <bluseio/utility/murmur_hash.h>
#include <bluseio/utility/os.h>
#include <bluseio/utility/pugixml.h>
#include <bluseio/utility/pystring.h>
#include <bluseio/utility/random.h>
#include <bluseio/utility/slab.h>
#include <bluseio/utility/uuid.h>
#include <bluseio/utility/datetime.h>
#include <bluseio/utility/json.h>
#include <bluseio/utility/spinlock.h>
#include <bluseio/utility/thread.h>
#include <bluseio/utility/threadpool.h>
#include <iostream>

using namespace bluseio::utility;

void fun1(int slp)
{
	printf("  hello, fun1 !  %d\n" ,std::this_thread::get_id());
	if (slp>0) {
		printf(" ======= fun1 sleep %d  =========  %d\n",slp, std::this_thread::get_id());
		std::this_thread::sleep_for(std::chrono::milliseconds(slp));
		//Sleep(slp );
	}
}

struct gfun {
	int operator()(int n) {
		printf("%d  hello, gfun !  %d\n" ,n, std::this_thread::get_id() );
		return 42;
	}
};

class A {    //函数必须是 static 的才能使用线程池
public:
	static int Afun(int n = 0) {
		std::cout << n << "  hello, Afun !  " << std::this_thread::get_id() << std::endl;
		return n;
	}

	static std::string Bfun(int n, std::string str, char c) {
		std::cout << n << "  hello, Bfun !  "<< str.c_str() <<"  " << (int)c <<"  " << std::this_thread::get_id() << std::endl;
		return str;
	}
};

int test_tp() {
	try {
		threadpool executor{ 50 };
		A a;
		std::future<void> ff = executor.commit(fun1,0);
		std::future<int> fg = executor.commit(gfun{},0);
		std::future<int> gg = executor.commit(a.Afun, 9999); //IDE提示错误,但可以编译运行
		std::future<std::string> gh = executor.commit(A::Bfun, 9998,"mult args", 123);
		std::future<std::string> fh = executor.commit([]()->std::string { std::cout << "hello, fh !  " << std::this_thread::get_id() << std::endl; return "hello,fh ret !"; });

		std::cout << " =======  sleep ========= " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::microseconds(900));

		for (int i = 0; i < 50; i++) {
			executor.commit(fun1,i*100 );
		}
		std::cout << " =======  commit all ========= " << std::this_thread::get_id()<< " idlsize="<<executor.idlCount() << std::endl;

		std::cout << " =======  sleep ========= " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));

		ff.get(); //调用.get()获取返回值会等待线程执行完,获取返回值
		std::cout << fg.get() << "  " << fh.get().c_str()<< "  " << std::this_thread::get_id() << std::endl;

		std::cout << " =======  sleep ========= " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));

		std::cout << " =======  fun1,55 ========= " << std::this_thread::get_id() << std::endl;
		executor.commit(fun1,55).get();    //调用.get()获取返回值会等待线程执行完

		std::cout << "end... " << std::this_thread::get_id() << std::endl;


		threadpool pool(4);
		std::vector< std::future<int> > results;

		for (int i = 0; i < 8; ++i) {
			results.emplace_back(
				pool.commit([i] {
					std::cout << "hello " << i << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(1));
					std::cout << "world " << i << std::endl;
					return i*i;
				})
			);
		}
		std::cout << " =======  commit all2 ========= " << std::this_thread::get_id() << std::endl;

		for (auto && result : results)
			std::cout << result.get() << ' ';
		std::cout << std::endl;
		return 0;
	}
	catch (std::exception& e) {
		std::cout << "some unhappy happened...  " << std::this_thread::get_id() << e.what() << std::endl;
	}
}

int main(void) {
	{
		// block_queue.h
		SyncTaskQueue<int> q(1024);
		std::cout << "sync q - " << "count: " << q.count() << std::endl;
		std::cout << "sync q - " << "empty: " << q.empty() << std::endl;
		std::cout << "sync q - " << "full: " << q.full() << std::endl;
		q.push(20);
		q.push(23);
		q.push(25);
		std::cout << "sync q - " << "count: " << q.count() << std::endl;
		int v = 0;
		q.pop(v);
		std::cout << "sync q - " << "v: " << v << std::endl;
		q.pop(v);
		std::cout << "sync q - " << "v: " << v << std::endl;
		q.pop(v);
		std::cout << "sync q - " << "v: " << v << std::endl;
		std::cout << "sync q - " << "count: " << q.count() << std::endl;
	}

	{
		// bloom_filter
		bluseio::utility::BloomFilter<90> bl;
		std::cout << "bloomfilter - " << "check: " << bl.check("hello", sizeof("hello")) << std::endl;
		bl.add("hello", sizeof("hello"));
		std::cout << "bloomfilter - " << "check: " << bl.check("hello", sizeof("hello")) << std::endl;
	}

	{
		MinTopHeap<int> heap;

		heap.insert(90);
		heap.insert(100);
		heap.insert(10);
		heap.insert(70);
		heap.insert(4);
		heap.insert(40);
		heap.insert(99);

		std::cout << "max min heap - " << " count : " << heap.size() << std::endl;
		std::cout << "max min heap - " << " extract : " << heap.extract() << std::endl;
		std::cout << "max min heap - " << " count : " << heap.size() << std::endl;
		std::cout << "max min heap - " << " top : " << heap.min() << std::endl;
		heap.show();
	}

	{
		random_t rand = {0};
		std::cout << "murmur - " << murmur_hash32("hello murmur", sizeof("hello murmur"), random(&rand)) << std::endl;
	}

	{
		std::cout << "uuid - " << uuid() << std::endl;
	}

	{
		char buf[1024*1024] = "";
		MemorySlab slab(buf, 1024*1024);

		void* p1 = slab.alloc(1024);
		void* p2 = slab.alloc(1024*4);
		void* p3 = slab.alloc(4);
		void* p4 = slab.alloc(1);

		if (buf != slab.address()) {
			std::cout << "slab - addr not match" << std::endl;
		}

		slab_stat_t st;

		slab.stat(st);
		std::cout << "slab - poolsize: " << st.pool_size << std::endl;
		std::cout << "slab - usedsize: " << st.used_size << std::endl;
		std::cout << "slab - usedpct: " << st.used_pct << std::endl;
		std::cout << "slab - pages: " << st.pages << std::endl;
		std::cout << "slab - freepages: " << st.free_page << std::endl;
		std::cout << "slab - psmall: " << st.p_small << std::endl;
		std::cout << "slab - pexact: " << st.p_exact << std::endl;
		std::cout << "slab - pbig: " << st.p_big << std::endl;
		std::cout << "slab - ppage: " << st.p_page << std::endl;
		std::cout << "slab - bsmall: " << st.b_small << std::endl;
		std::cout << "slab - bexact: " << st.b_exact << std::endl;
		std::cout << "slab - bbig: " << st.b_big << std::endl;
		std::cout << "slab - bpage: " << st.b_page << std::endl;
		std::cout << "slab - max frees pages: " << st.max_free_pages << std::endl;
	}

	{
		// datetime
		DateTime dt;

		std::cout << "datetime - " << dt.valid() << std::endl;
		std::cout << "datetime - " << dt.duration_str() << std::endl;
		std::cout << "datetime - " << dt.datetime_str() << std::endl;
		std::cout << "datetime - " << dt.iso8601_str() << std::endl;
		std::cout << "datetime - " << dt.file_str() << std::endl;
		std::cout << "datetime - " << dt.logtime_str() << std::endl;
		std::cout << "datetime - " << dt.date_str() << std::endl;
		std::cout << "datetime - " << dt.seconds() << std::endl;

		std::cout << "datetime - clock: " << DateTime::clock() << std::endl;
		std::cout << "datetime - tickms: " << DateTime::tick_ms() << std::endl;
		std::cout << "datetime - tickus: " << DateTime::thrtime_us() << std::endl;
		std::cout << "datetime - nowus: " << DateTime::now_us() << std::endl;
		std::cout << "datetime - nowms: " << DateTime::now_ms() << std::endl;
		std::cout << "datetime - nowsec: " << DateTime::now_sec() << std::endl;

	}

	{
		Json null_json;
		std::cout << null_json.format_str() << std::endl;

		Json object_json(JSON_OBJECT);
		object_json["key1"] = "1234";
		object_json["key2"] = "1234";
		object_json["key3"] = Json(JSON_OBJECT);
		object_json["key4"] = true;
		object_json["key5"] = 45;
		object_json["key6"] = Json(JSON_ARRAY);

		std::cout << object_json.format_str() << std::endl;
	}

	{
		test_tp();
	}
	
	return 0;
}