    /*
    * CH2: ch2.cpp
    *
    *  Created on: 2022. ?. ?.
    *      Author: your name
    *
    *
    *  + UI::getNext(), UI::getNextLine() 추가
    *  + Person::passwd, getPasswd(), setPasswd() 추가
    *  + VectorPerson 클래스 추가: 객체 포인터 배열의 생성 및 소멸
    *  + PersonManager 클래스 추가: 객체 배열의 복사, 객체의 동적 생성 및 소멸
    *  + MultiManager 클래스: 객체 배열로 변경
    */
    #include <iostream>
    #include <cstring>

    using namespace std;  // 헤드 파일은 반드시 이 문장 앞쪽에 include해야 한다.

    /******************************************************************************
     * 아래 상수 정의는 필요에 따라 변경하여 사용하라.
     ******************************************************************************/
    #define AUTOMATIC_ERROR_CHECK false // false: 자동 오류 체크, true: 키보드에서 직접 입력하여 프로그램 실행

    /******************************************************************************
     * Person structure and its manipulation functions
     ******************************************************************************/
    class Person
    {
        string name;            // 이름: 변경
        int    id;              // Identifier
        double weight;          // 체중
        bool   married;         // 결혼여부
        char   address[40];     // 주소

    protected:
        void inputMembers(istream* in);
        void printMembers(ostream* out);

    public:
        Person();
        Person(const string name);
        Person(const string name, int id, double weight, bool married, const char *address);
        ~Person();

        void set(const string pname, int pid, double pweight, bool pmarried, const char *paddress);
        void setName(const string pname)      { name = pname; }
        void setId(int pid)                   { id = pid; }
        void setWeight(double pweight)        { weight = pweight; }
        void setMarried(bool pmarried)        { married = pmarried; }
        void setAddress(const char* paddress) { strcpy(address, paddress); }

        const string getName()    { return name; }
        int         getId()      { return id; }
        double      getWeight()  { /* TODO: [문제 2] */ return weight; }  // 구현 시
        bool        getMarried() { /* TODO: [문제 2] */ return married; }  // 리턴 값들을
        const char* getAddress() { /* TODO: [문제 2] */ return address; } // 수정하시오.

        void input(istream* pin)  { inputMembers(pin); } // ch3_2에서 추가
        void print(ostream* pout) { printMembers(pout); }
        void println()            { print(&cout); cout << endl; }
        void whatAreYouDoing();                          // ch3_2에서 추가
        bool isSame(const string name, int pid);         // ch3_2에서 추가
    };

    Person::Person(): Person("") {
        // 위 함수 서두(:와 함수 본체 사이)에서 각 멤버를 초기화하는데 이는 함수 진입하기 전에 
        // 각 멤버의 값을 초기화하는 것이다. {}는 각 데이타 타입별로 디폴트 값으로 초기화하라는 의미임. 
        // 즉, name[]={'\0'}="", id=0, weight=0.0, married=false, address[]={'\0'}=""

        //cout << "Person::Person():"; println();
    }

    Person::Person(const string name): Person("",0,0,0,"") /* : TODO [문제 2] */ {
        // TODO: [문제 2]

        // setName(name);
        // setAddress(address);
        // cout << "Person::Person(\"" << name << "\"):"; println();
    }

    Person::Person(const string name, int id, double weight, bool married, const char *address) :
            name{name}, id{id}, weight{weight}, married{married} {
        // 위에서 각 멤버를 초기화하는 {}는 각 매개변수 값을 객체의 상응하는 멤버에 설정하는 것이다. 즉,
        // this->id=id, this->weight=weight, this->married=married와 동일하다.
        // 여기서 this는 해당 객체를 포인터하는 포인터 변수이며, (다음 장에서 설명될 예정임)
        // this->id는 해당 객체의 멤버 id를 지칭하며 =의 우측변수 id는 함수의 매개변수이다.
        // 값을 간단히 대입하는 것은 위처럼 하면 되지만 name[], address[]와 같은 배열 복사 함수 
        // 호출 또는 다른 함수를 호출할 때는 생성자 함수의 몸체 { } 내에서 호출해야 한다.


        setName(name);
        setAddress(address);
        // TODO: [문제 2] 
        cout << "Person::Person(...):"; println();
    }

    Person::~Person() {
        cout << "Person::~Person():"; println();
    }

    void Person::printMembers(ostream* pout)   {
        // TODO: [문제 1]
        *pout<<name<<' '<<id<<' '<<weight<<' '<<married<<" :"<<address<<':';
    }

    void Person::set(const string pname, int pid, double pweight,
        bool pmarried, const char *paddress) {
        // TODO: [문제 4]
        name = pname;
        id = pid;
        weight = pweight;
        married = pmarried;
        setAddress(paddress);
    }

    void Person::inputMembers(istream* pin)   {
        // TODO: [문제 7]
        *pin>>name>>id>>weight>>married;
        pin->getline(address, sizeof(address), ':'); // 말씀하셨던 버퍼에 \n이 남아서 그걸 치우고 입력받기위해 두번씀
        pin->getline(address, sizeof(address), ':');
    }

    void Person::whatAreYouDoing() {
        // TODO: [문제 5]
        cout<<name<<" is taking a rest.\n";
    }

    bool Person::isSame(const string pname, int pid) {
        // TODO: [문제 6]
        return name == pname && id == pid;
    }


    /******************************************************************************
     * User Interface
     ******************************************************************************/
    // 기본적인 입력과 관련된 전역 함수들을 UI라는 이름공간 내부에 정의함
    
    namespace UI {

    bool echo_input = false;
    string line, emptyLine;

    // 입력에서 정수 대신 일반 문자가 입력되었는지 체크하고 에러 발생시 에러 메시지 출력
    bool checkInputError(istream* pin, const string msg) {
        if (!(*pin)) { // 에러가 발생했다면
            cout << msg;  // 에러 메시지를 출력
            pin->clear(); // 에러 발생 상태정보를 리셋함; 그래야 다음 문장에서 읽을 수 있음
            getline(*pin, emptyLine); // 에러가 발생한 행 전체를 읽어 데이터를 버림
            return true;
        }
        return false;
    }

    // 정수나 실수를 입력해야 하는 곳에 일반 문자열을 입력한 경우의 에러 체크
    bool checkDataFormatError(istream* pin) {
        return checkInputError(pin, "Input-data format MISMATCHED\n");
    }

    // 한 사람의 정보 즉, 각 멤버 데이터를 순서적으로 입력 받아 p에 저장하고
    // 입력 중 입력 데이터에 오류가 있는지 확인하고 오류가 있을 시 에러 메시지를 출력한다.
    bool inputPerson(Person* p) {
        cout << "input person information:" << endl;
        p->input(&cin);
        if (checkDataFormatError(&cin)) return false;
        if (echo_input) p->println(); // 자동체크에서 사용됨
        return true;
    }

    // 하나의 정수를 입력 받음; 정수가 아닌 아닌 문자열 입력시 에러 메시지 출력 후 재입력 받음
    int getInt(const string msg) {
        for (int value; true; ) {
            cout << msg;
            cin >> value;
            if (echo_input) cout << value << endl; // 자동체크 시 출력됨
            if (checkInputError(&cin, "Input an INTEGER.\n"))
                continue;
            getline(cin, emptyLine); // skip [enter] after the number
            return value;
        }
    }

    // 하나의 양의 정수를 입력 받음; 음수 입력시 에러 메시지 출력 후 재입력 받음
    int getPositiveInt(const string msg) {
        int value;
        while ((value = getInt(msg)) < 0)
            cout << "Input a positive INTEGER." << endl;
        return value;
    }

    // 0~(size-1)사이의 선택된 메뉴 항목 또는 리스트의 항목의 인덱스 값을 리턴함
    // 존재하지 않는 메뉴항목을 선택한 경우 에러 출력
    int getIndex(const string msg, int size) {
        while (true) {
            int index = getPositiveInt(msg);
            if (0 <= index  && index < size) return index;
            cout << index << ": OUT of selection range(0 ~ "
                << size-1 << ")" << endl;
        }
    }

    // 사용자에게 메뉴를 보여주고 사용자가 선택한 메뉴항목의 인덱스를 리턴함
    int selectMenu(const string menuStr, int menuItemCount) {
        cout << endl << menuStr;
        return getIndex("Menu item number? ", menuItemCount);
    }

    } // namespace UI

    /******************************************************************************
     * ch3_2: CurrentUser class
     ******************************************************************************/

    class CurrentUser
    {
        Person* pUser;

    public:
    //  CurrentUser(Person u): user(u) { }
        CurrentUser(Person* pUser): pUser(pUser)/* TODO [문제 1]:  멤버 초기화 */ { }
        void display();
        void setter();
        void getter();
        void set();
        void whatAreYouDoing();
        void isSame();
        void inputPerson();
        void run();
    };

    void CurrentUser::display() { // Menu item 1
        pUser->println();
    }

    void CurrentUser::getter() { // Menu item 2
        cout << "name:" << pUser->getName() << ", id:" << pUser->getId() << ", weight:" <<
                pUser->getWeight() << ", married:" << pUser->getMarried() <<
                ", address:" << pUser->getAddress() << endl;
    }

    void CurrentUser::setter() { // Menu item 3
        Person* pp = new Person("pp");  /* TODO [문제 3]: 동적 메모리 할당 */
        pp->setName(pp->getName());
        pp->setId(pUser->getId());
        pp->setWeight(pUser->getWeight());
        pp->setMarried(pUser->getMarried());
        pp->setAddress(pUser->getAddress());
        cout << "pp->setMembers():"; pp->println();

        delete pp;
    }

    void CurrentUser::set() { // Menu item 4
        Person* pp = new Person("pp");  /* TODO [문제 3]: 동적 메모리 할당 */
        pp->set(pp->getName(), pUser->getId(), pUser->getWeight(),
                !pUser->getMarried(), pUser->getAddress());
        cout << "pp->set():"; pp->println();

        delete pp;
    }

    void CurrentUser::whatAreYouDoing() {  // Menu item 5
        pUser->whatAreYouDoing();
    }

    void CurrentUser::isSame() { // Menu item 6
        pUser->println();
        cout << "isSame(\"user\", 1): " << pUser->isSame("user", 1) << endl;
    }

    void CurrentUser::inputPerson() { // Menu item 7
        if (UI::inputPerson(pUser)) // GilDong 1 70.5 true :Jongno-gu, Seoul:
            display();              // user 1 71.1 true :Gwangju Nam-ro 21:
    }

    void CurrentUser::run() {
        using func_t = void (CurrentUser::*)();
        func_t func_arr[] = {
            nullptr, &CurrentUser::display, &CurrentUser::getter, &CurrentUser::setter,
            &CurrentUser::set, &CurrentUser::whatAreYouDoing,
            &CurrentUser::isSame, &CurrentUser::inputPerson,
        };
        int menuCount = sizeof(func_arr) / sizeof(func_arr[0]); // func_arr[] 배열의 길이
        string menuStr =
            "+++++++++++++++++++++ Current User Menu ++++++++++++++++++++++++\n"
            "+ 0.Logout 1.Display 2.Getter 3.Setter 4.Set 5.WhatAreYouDoing +\n"
            "+ 6.IsSame 7.InputPerson                                       +\n"
            "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

        while (true) {
            int menuItem = UI::selectMenu(menuStr, menuCount);
            if (menuItem == 0) return;
            (this->*func_arr[menuItem])();
        }
    }
    // ch3_2: CurrentUser class

/******************************************************************************
 * ch4_2: VectorPerson class
 ******************************************************************************/
// VectorPerson는 n개의 Person * 포인터를 pVector[] 배열에 담아 관리하는 클래스이다.
// 필요에 따라 포인터를 삽입, 삭제, 추가 등을 할 수 있다.
// 삽입하는 포인터 개수가 점점 많아지면 pVector[] 배열을 위한 메모리를 자동 확장하기도 한다.

class VectorPerson
{
    static const int DEFAULT_SIZE = 10; // pVector의 디폴트 배열 원소 개수

    Person **pVector; // Person *pVector[]; Person에 대한 포인터들의 배열, 배열에 저장될 값이 Person *이다.
    int count;        // pVector 배열에 현재 삽입된 객체 포인터의 개수
    int allocSize;    // 할당 받의 pVector의 총 배열 원소의 개수

    void extend_capacity(); /* TODO 문제 [7] */

public:
    VectorPerson():VectorPerson(DEFAULT_SIZE) {  /* TODO 문제 [2]: 위임 생성자로 변환 */
        // allocSize = DEFAULT_SIZE;
        // count = 0;
        // cout << "VectorPerson::VectorPerson(" << allocSize << ")" << endl;
        // pVector = new Person*[allocSize]; // 객체 포인터들의 배열을 위한 동적 메모리 할당
    }
    VectorPerson(int capacity);
    ~VectorPerson();

    // 아래 긱 함수이름 뒤의 const는 그 함수가 클래스 멤버 변수들을 수정하지 않고 읽기만 한다는 의미임
    // pVector[index]의 포인터 값을 반환 
    Person* at(int index) const { /* TODO 문제 [1] */ return pVector[index]; }

    // 할당 받의 pVector의 총 배열 원소의 개수를 반환
    int     capacity()    const { /* TODO 문제 [1] */ return allocSize; }

    // pVector 배열에 현재 삽입된 객체 포인터의 개수를 0으로 설정
    void    clear()             { /* TODO 문제 [1] */count = 0; }

    // 현재 삽입된 객체 포인터가 하나도 없으면 true, 있으면 false
    bool    empty()       const { /* TODO 문제 [1] */ return count==0; }

    // 현재 삽입된 객체 포인터의 개수를 반환
    int     size()        const { /* TODO 문제 [1] */ return count; }

    // pVector 배열에 마지막 삽입된 원소 뒤에 새로운 원소 p를 삽입하고 현재 삽입된 객체 개수를 증가
    void    push_back(Person* p); /* TODO 문제 [4, 7] */
};

void VectorPerson::push_back(Person* p){
    this->pVector[this->size()] = p; //카운트 넣어도 돌아감
    count += 1; 
}

// capacity는 할당해야 할 배열 원소의 개수
VectorPerson::VectorPerson(int capacity): allocSize{capacity}, count{0}/* : TODO 문제 [2]: 멤버 초기화 */ {
    // allocSize = capacity, count = 0; 초기화를 위 함수 서두(위 /* */ 주석 사이)에서 할 것
    // 함수 서두에서 초기화하는 방법은 Person 클래스 참고할 것
    cout << "VectorPerson::VectorPerson(" << allocSize << ")" << endl;
    pVector = new Person*[allocSize]; // Person* 들의 배열을 위한 동적 메모리 할당
}

VectorPerson::~VectorPerson()  {
    /* TODO 문제 [2]: 동적으로 할당된 배열 pVector 반납: pVector가 배열임을 명심하라. */
    delete[] pVector;
    cout << "VectorPerson::~VectorPerson(): pVector deleted" << endl;
}

/******************************************************************************
 * ch4_2: PersonManager class
 ******************************************************************************/

class PersonManager
{
    VectorPerson persons;

    void deleteElemets();
    void printNotice(const string preMessage, const string postMessage);
    Person* findByName(const string name);

public:
    PersonManager(Person* array[], int len); // 6장에서 default 매개변수 설정
    ~PersonManager();
    void display();
    void append();
    void clear();
    void login();
    void run();
};

PersonManager::PersonManager(Person* array[], int len) {
    cout << "PersonManager::PersonManager(array[], len)" << endl;
    /* TODO 문제 [4] */
    for( int i=0; i<len; i++ ){
        Person *s = array[i];
        Person *tmp = new Person(s->getName(),s->getId(),s->getWeight(),s->getMarried(),s->getAddress());
        persons.push_back(tmp);
    }

    display();
}

PersonManager::~PersonManager() {
    deleteElemets();
    display();
}

void PersonManager::deleteElemets() {
    /* TODO 문제 [5] */
    
    for(int i=0; i<persons.size(); i++){
        
    }
    persons.clear();
}

void PersonManager::display() { // Menu item 1
    int count = persons.size();
    cout << "display(): count " << count << endl;
    for (int i = 0; i < count; ++i) {
        cout << "[" << i << "] ";
        persons.at(i)->println();
    }
    cout << "empty():" << persons.empty() << ", size():" << persons.size()
         << ", capacity():" << persons.capacity() << endl;
}

void PersonManager::append() { /* TODO 문제 [6] */ } // Menu item 2

void PersonManager::clear() { // Menu item 3
    deleteElemets();
    display();
}
void PersonManager::login() { // Menu item 4 
    /* TODO 문제 [8] */ 
}

void PersonManager::run() {
    using func_t = void (PersonManager::*)();
    using PM = PersonManager; // 코딩 길이를 줄이기 위해
    func_t func_arr[] = {
        nullptr, &PM::display, &PM::append, &PM::clear, &PM::login,
    };
    int menuCount = sizeof(func_arr) / sizeof(func_arr[0]); // func_arr[] 길이
    string menuStr =
        "====================== Person Management Menu ===================\n"
        "= 0.Exit 1.Display 2.Append 3.Clear 4.Login(CurrentUser, ch4_2) =\n"
        "=================================================================\n";

    while (true) {
        int menuItem = UI::selectMenu(menuStr, menuCount);
        if (menuItem == 0) return;
        (this->*func_arr[menuItem])();
    }
}

    /******************************************************************************
 * ch3_2, 4_1, 4_2: MultiManager class
 ******************************************************************************/
class MultiManager
{
    static const int personCount = 5;
    Person persons[personCount] = {
        Person("p0", 10, 70.0, false, "Gwangju Nam-gu Bongseon-dong 21"),
        Person("p1", 11, 61.1, true,  "Jong-ro 1-gil, Jongno-gu, Seoul"),
        Person("p2", 12, 52.2, false, "1001, Jungang-daero, Yeonje-gu, Busan"),
        Person("p3", 13, 83.3, true,  "100 Dunsan-ro Seo-gu Daejeon"),
        Person("p4", 14, 64.4, false, "88 Gongpyeong-ro, Jung-gu, Daegu"),
    };
    // new를 이용해 동적으로 할당할 경우 소멸자 함수를 만들어 거기서 delete 해 주어야 함

    static const int allPersonCount = personCount;
    Person* allPersons[allPersonCount] = {
        &persons[0], &persons[1], &persons[2], &persons[3], &persons[4],
    };

    PersonManager personMng { allPersons, allPersonCount };
    // 위 문장은 PersonManager personMng(allPersons, allPersonCount);와 동일함

public:
    void run() {
        cout << "PersonManager::run() starts" << endl;
        personMng.run();
        cout << "PersonManager::run() returned" << endl;
    }
}; // ch3_2, 4_1, 4_2: MultiManager class

    /******************************************************************************
     * ch3_1: Class And Object
     ******************************************************************************/

    //Person gps("gps", 1000, 100, true, "gps adress");

    class ClassAndObject
    {
        void defualConstructor() { // Menu item 1
            Person ps;    // Person::Person() 기본 생성자 및 소멸자 실행
        }

        void constructor() { // Menu item 2: 지역객체의 다양한 생성자 및 소멸자 실행
            Person ps1;
            Person ps2("ps2");
            Person ps3("ps3", 3, 70.3, true, "ps3 address");
        }

        void construcor_destructor_inner_inner() {
            cout << "\n--- construcor_destructor_inner_inner() begins ---" << endl;
            Person ps3("ps3");
            cout << "\n- if begins -" << endl;
            if (true) {
                Person ps4("ps4");
            }
            cout << "- if ends -\n" << endl;
            cout << "- temporary object begins -" << endl;
            Person("temp_ps_1"); // 임시객체(이름 없는 객체) 성성 및 소멸
            cout << endl;
            Person("temp_ps_2").setName("TEMP_PS_2"); // 임시객체 생성, 
                                                    // setName() 호출, 임시객체 소멸
            cout << "- temporary object ends -\n" << endl;
            Person ps5("ps5");
        }

        void construcor_destructor_inner() {
            cout << "\n--- construcor_destructor_inner() begins ---" << endl;
            Person ps2("ps2");
            construcor_destructor_inner_inner();
            cout << "--- construcor_destructor_inner_inner() returned ---\n" << endl;
            Person ps6("ps6");
        }

        void construcorDestructor() { // Menu item 3
            Person ps1("ps1");
            construcor_destructor_inner();
            cout << "--- construcor_destructor_inner() returned ---\n" << endl;
        }

        void global_static_local_objects_inner() {
            cout << "\n--- global_static_local_objects_inner() begins ---" << endl;
            //static Person sps("sps", 2000, 200, false, "sps adress");
        }

        void globalStaticLocalObjects() { // Menu item 4
            global_static_local_objects_inner();
            cout << "--- global_static_local_objects_inner() returned ---" << endl;
            global_static_local_objects_inner();
            cout << "--- global_static_local_objects_inner() returned ---" << endl;
        }

    public:
        // 사용자가 선택한 메뉴 항목을 실행하는 멤버 함수(func_arr[menuItem]에 등록된 함수)를 호출
        void run() {
            // ClassAndObject의 멤버 함수에 대한 포인터 타입인 새로운 데이타 타입 func_t를 정의함
            using func_t = void (ClassAndObject::*)();
            // 위 using 문은 ClassAndObject 클래스의 멤버 함수(리턴 타입이 void 이면서 매개변수가 없는)에
            // 대한 포인터 타입을 앞으로는 간단히 func_t로 사용하겠다는 의미이다.

            // ClassAndObject의 멤버 함수 포인터들의 배열
            // 나중에 호출하기 위해 멤버 함수의 이름을 미리 배열에 저장(등록) 
            //     함수 이름이 곧 함수에 대한 포인터임: 즉, 함수의 첫번째 명령어의 주소를 저장함
            // 등록된 이 멤버 함수들은 나중에 run()에서 호출됨
            //     (즉, 함수 주소로 jump 해 가서 함수를 실행함)

            func_t func_arr[] = { // 메뉴항목을 실행하는 멤버 함수를 배열에 미리 저장(등록)해 둠
                nullptr, &ClassAndObject::defualConstructor, &ClassAndObject::constructor,
                &ClassAndObject::construcorDestructor, &ClassAndObject::globalStaticLocalObjects,
            };
            int menuCount = sizeof(func_arr) / sizeof(func_arr[0]); 
            // func_arr[]의 원소의 개수 = 배열 전체 크기 / 한 배열 원소의 크기

            // 화면에 보여 줄 메뉴
            string menuStr =
                "+++++++++++ Person Class And Object Menu ++++++++++++\n"
                "+ 0.Exit 1.DefualConstructor 2.Constructor          +\n"
                "+ 3.ConstrucorDestructor 4.GlobalStaticLocalObjects +\n"
                "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

            while (true) {
                int menuItem = UI::selectMenu(menuStr, menuCount); // 메뉴 번호 입력 받음
                if (menuItem == 0) return;
                (this->*func_arr[menuItem])(); // 선택된 메뉴 항목을 실행할 멤버 함수를 호출함
                /* 위 문장은 다음을 간단히 한 것이다. 앞으로 계속 나올테니 잘 이해하기 바란다.
                func_t f = func_arr[menuItem];// 배열에 저장된 멤버 함수 포인터
                (this->*f)();                 // 멤버함수 포인터를 이용하여 해당 함수를 호출함
                */
            }
        }
    }; // ch3_1: class ClassAndObject


    /******************************************************************************
     * Main Menu
     ******************************************************************************/

    class MainMenu
    {
    public:
        void run() {
            int menuCount = 3; // 상수 정의
            string menuStr =
"******************************* Main Menu *********************************\n"
"* 0.Exit 1.PersonManager(ch3_2, 4)                                        *\n"
"* 2.Class:Object(ch3_1)                                                   *\n"
"***************************************************************************\n";

            while (true) {
                int menuItem = UI::selectMenu(menuStr, menuCount);
                if (menuItem == 0) break;

                switch(menuItem) {
                case 1: MultiManager().run();             break;
                case 2: ClassAndObject().run();           break;
                }
            }
            cout << "Good bye!!" << endl;
        }
    }; // class MainMenu

    /******************************************************************************
     * run() 함수
     ******************************************************************************/

    void run() {
        MainMenu().run();
        // MainMenu 타입의 이름 없는 임시객체를 생성한 후 
        // 그 객체의 run() 멤버함수를 호출함; run()에서 리턴한 후에는 임시객체가 자동 소멸됨
        // 즉, 위 문장은 아래 두 문장과 동일한 기능임
        // MainMenu mm;
        // mm.run();
    }

    /******************************************************************************
     * Program Test: Automatic Error Check
     ******************************************************************************/
    #if AUTOMATIC_ERROR_CHECK
    #include "check_error.h"
    #endif

    /******************************************************************************
     * main() 함수
     ******************************************************************************/
    int main() {
        cout << boolalpha;  // 11장에서 배움; bool 타입 값을 0, 1 대신 true, false로 출력하도록 설정
        cin >> boolalpha;   // bool 타입 값을 0, 1 대신 true, false로 입력 받도록 설정

    #if AUTOMATIC_ERROR_CHECK
        evaluate(false);   // 각 문제에 대해 단순히 O, X만 확인하고자 할 때는 false
    #else
        run();
    #endif
    }