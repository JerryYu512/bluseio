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
#include <iostream>

using namespace bluseio::utility;

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
	
	return 0;
}