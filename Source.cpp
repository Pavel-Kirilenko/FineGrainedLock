#include<mutex>

struct Node
{
	int value;
	Node* next;
	std::mutex* node_mutex;
	Node(int val) : value(val), next(nullptr), node_mutex(new std::mutex) {}   // конструктор структуры с инициализицией значения
	~Node()                                                                    // деструктор структуры для предотвращения утечек памяти
	{
		delete node_mutex;
	}
};

class FineGrainedQueue
{
	Node* head;
	std::mutex* queue_mutex;
public:
	FineGrainedQueue(Node* node) : head(node), queue_mutex(new std::mutex) {}  // конструктор класса с инициализацией структурой 'Node'
	~FineGrainedQueue()                                                        // деструктор класса для предотвращения утечек памяти
	{
		delete queue_mutex;
	}
	void insertIntoMiddle(int value, int pos)
	{
		pos -= 2;                              // откорректировать номер позиции в алгоритме с учетом психологических особенностей человеческого восприятия
		Node* newNode = new Node(value);       // определить вставляемый узел и инициализировать его заданным значением
		queue_mutex->lock();                   // залочить мьютекс очереди целиком
		Node* curNode = head;                  // присвоить вспомогательному текущему узлу значение 'head'
		for (int i = 0; i < pos; i++)          // двигаться по списку до узла с заданным номером или до конца списка
		{
			if (!curNode->next) continue;      // выйти из цикла, если достигнут конец списка
			curNode = curNode->next;           // перейти к следующему узлу
		}
		curNode->node_mutex->lock();           // залочить мьютекс текущего элемента
		queue_mutex->unlock();                 // разлочить мьютекс очереди
		newNode->node_mutex->lock();           // залочить мьютекс вставляемого элемента для дальнейших операций вставки
		if (curNode->next)                     // если дальше по списку есть узел/узлы
		{
			newNode->next = curNode->next;     // вставить узел между узлами с инициализацией указателя на последующий узел
			curNode->next = newNode;

		}
		else                                   // иначе
		{
			curNode->next = newNode;           // вставить узел в конец списка без инициализации указателя на следующий элемент 
		}
		newNode->node_mutex->unlock();         // разлочить мьютекс вставленного узла
		curNode->node_mutex->unlock();         // разлочить мьютекс предыдущего узла
	};
};

int main()
{
	Node* node = new Node(0);                  // определить узел с иннициализацией значения
	FineGrainedQueue queue(node);              // создать ненулевой свисок
	for (int i = 2; i < 6; i++)
	{
		queue.insertIntoMiddle(i, i);          // вставить узлы в список в различные позицие с различными значениями
	}

	queue.insertIntoMiddle(100, 2);
	queue.insertIntoMiddle(200, 1000);

	return 0;                                  // рекомендуемая точка останова для проверки работы функции 'insertIntoMiddle'
}






