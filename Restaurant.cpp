#include "main.h"

class customerOrder{
public:
    Restaurant::customer* data;
    customerOrder* next;
    customerOrder* prev;
    bool QueueCheck;
    customerOrder(Restaurant::customer* data, bool QueueCheck = false, customerOrder* next = nullptr, customerOrder* prev = nullptr):
            data(data), QueueCheck(QueueCheck), next(next), prev(prev){}
    ~customerOrder(){
        if (data != nullptr){
            delete data;
            COUNTDELETE += 1;
        }
    }
};

class imp_res : public Restaurant
{
protected:
    customer* TableOrder; //! DanhSachLienKetVong
    customer* CustomerX; //! Khách hàng ở vị trí X
    customer* GuestQueue; //! Hàng chờ
    //! Head và tail cho thứ tự khách hàng
    customerOrder* orderHead;
    customerOrder* orderTail;
    //! Head và tail cho thứ tự khách hàng
    int GuestInQueue;
    int GuestInTable;
public:
    bool CheckDuplicate(customer*); //!"thiên thượng thiên hạ, duy ngã độc tôn"

    /* Hàm sử dụng cho khách ngồi trong bàn ăn */
    void AppendGuestTable(customer*); //!Quăng khách vào bàn
    void RemoveGuestOrder(int, customerOrder*&, customerOrder*&);
    /* Hàm sử dụng cho khách ngồi trong bàn ăn */

    /*Hàm sử dụng cho khách nằm trong hàng chờ*/
    void AppendGuestQueue(customer*); //!Quăng khách vào hàng chờ
    customer* PopGuest(); //!Xóa phần tử đầu trong hàng chờ
    customer* PopGuest(customer*);
    /*Hàm sử dụng cho khách nằm trong hàng chờ*/

    /*Hàm sử dụng để kiểm tra thứ tự khách hàng*/
    void AppendOrder(customerOrder*&, customerOrder*&, customer* );
    void ClearOrder(customerOrder*&, customerOrder*&);
    /*Hàm sử dụng để kiểm tra thứ tự khách hàng*/

    void DeleteType(customerOrder*& head, customerOrder*& tail, int &length);
    void SwapNode(customer*&, customer*&);
    customer* findMin(customer* start, customer* end, int length);
public:
    imp_res() {
        TableOrder = nullptr;
        GuestQueue = nullptr;
        GuestInTable = 0;
        GuestInQueue = 0;
        CustomerX = nullptr;
        orderHead = orderTail = nullptr;
    };
    ~imp_res(){
        while(orderHead){
            customerOrder*temp = orderHead;
            orderHead = orderHead -> next;
            delete temp;
            COUNTDELETE += 1;
        }
        if (GuestQueue != nullptr) {
            GuestQueue->prev->next = nullptr;
            GuestQueue->prev = nullptr;
            while (GuestInQueue) {
                customer *temp = GuestQueue;
                GuestQueue = GuestQueue->next;
                GuestInQueue -= 1;
                delete temp;
                COUNTDELETE += 1;
            }
        }
        TableOrder = nullptr;
        GuestQueue = nullptr;
        GuestInTable = 0;
        GuestInQueue = 0;
        CustomerX = nullptr;
        orderHead = orderTail = nullptr;
    }
    void RED(string name, int energy)
    {
        //cout << name << " " << energy << endl;
        customer *cus = new customer (name, energy, nullptr, nullptr);
        COUNTDELETE -= 1;
        if (cus->energy == 0){
            delete cus;
            COUNTDELETE += 1;
            return; //! Không phải chú thuật sư hay chú linh
        }
        if (CheckDuplicate(cus)){
            delete cus;
            COUNTDELETE += 1;
            return; //! "thiên thượng thiên hạ, duy ngã độc tôn"
        }
        if (GuestInTable == MAXSIZE){ //! Thêm khách vào hàng chờ
            if (GuestInQueue == MAXSIZE){
                delete cus;
                COUNTDELETE += 1;
                return;
            }
            AppendGuestQueue(cus);
            return;
        }
        AppendGuestTable(cus); //! Thêm khách vào bàn ăn
    }
    void BLUE(int num)
    {
        //cout << "blue "<< num << endl;
        if (GuestInTable == 0) return;
        if (num >= MAXSIZE || num >= GuestInTable) RemoveGuestOrder(GuestInTable, orderHead, orderTail);
        else RemoveGuestOrder(num, orderHead, orderTail);
        while (GuestInQueue != 0 && GuestInTable < MAXSIZE){
            customer* FirstGuest = PopGuest();
            AppendGuestTable(FirstGuest);
        }
    }
    void PURPLE()
    {
        //cout << "purple"<< endl;
    }
    void REVERSAL()
    {
        //cout << "reversal" << endl;
        if (GuestInTable == 1 || GuestInTable == 0) return;
        int Sorcerers = 0, Spirits = 0;
        customer* temp = CustomerX;
        for (int i = 0; i < GuestInTable; i++){
            if (temp -> energy > 0) Sorcerers += 1;
            else Spirits += 1;
            temp = temp -> next;
        }
        temp = CustomerX;
        customer* head = nullptr; customer* tail = nullptr;
        //! Đảo ngược oán linh trước
        for (int i = 0; i < GuestInTable; i++){
            if (temp -> energy < 0){
                head = temp;
                break;
            }
            temp = temp -> prev;
        }
        int num = 0;
        if (head != CustomerX){
            temp = CustomerX;
            num = GuestInTable;
        }
        else{
            temp = CustomerX -> next;
            num = GuestInTable - 1;
        }
        for (int i = 0; i < num; i++){
            if (temp -> energy < 0){
                tail = temp;
                break;
            }
            temp = temp -> next;
        }
        if (head != tail && Spirits > 1) {
            while(true){
                SwapNode(head, tail);
                head = head -> prev;
                while(head -> energy > 0){
                    head = head -> prev;
                }
                if (head == tail) break;
                tail = tail -> next;
                while(tail -> energy > 0){
                    tail = tail -> next;
                }
                if (tail == head) break;
            }
            SwapNode(head, tail);
            while (true){
                if (head == tail) break;
                if (head->energy < 0 && tail->energy < 0) {
                    SwapNode(head, tail);
                    head = head->prev;
                    if (head == tail) break;
                    tail = tail->next;
                    if (head == tail) break;
                }
                if (head -> energy > 0) head = head->prev;
                if (head == tail) break;
                if (tail -> energy > 0) tail = tail->next;
                if (head == tail) break;
            }
        }
        //! Đảo ngược oán linh trước
        head = nullptr; tail = nullptr; num = 0; temp = CustomerX;
        //! Đảo ngược chú thuật sư
        for (int i = 0; i < GuestInTable; i++){
            if (temp -> energy > 0){
                head = temp;
                break;
            }
            temp = temp -> prev;
        }
        if (head != CustomerX){
            temp = CustomerX;
            num = GuestInTable;
        }
        else{
            temp = CustomerX -> next;
            num = GuestInTable - 1;
        }
        for (int i = 0; i < num; i++){
            if (temp -> energy > 0){
                tail = temp;
                break;
            }
            temp = temp -> next;
        }
        if (head != tail && Sorcerers > 1) {
            while(true){
                SwapNode(head, tail);
                head = head -> prev;
                while (head -> energy < 0){
                    head = head -> prev;
                }
                if (head == tail) break;
                tail = tail -> next;
                while (tail -> energy < 0){
                    tail = tail -> next;
                }
                if (tail == head) break;
            }
        }
        //! Đảo ngược chú thuật sư
    }
    void UNLIMITED_VOID()
    {
        //cout << "unlimited_void" << endl;
        if (GuestInTable <= 3) return;
        int MinEnergy = INT_MAX, AnsElementCount = 0;
        customer* temp = CustomerX;
        customer* start = nullptr; customer* end = nullptr;
        for (int i = 0; i < GuestInTable; i++){
            int TotalEnergy = 0, ElementCount = 0;
            customer* temp_J = temp;
            for (int j = i; j < GuestInTable + i; j++){
                TotalEnergy += temp_J -> energy;
                ElementCount += 1;
                if (ElementCount >= 4){
                    if (start == nullptr && end == nullptr){
                        start = temp; end = temp_J;
                        MinEnergy = TotalEnergy;
                        AnsElementCount = ElementCount;
                    }
                    else if (MinEnergy >= TotalEnergy){
                        MinEnergy = TotalEnergy;
                        start = temp; end = temp_J;
                        AnsElementCount = ElementCount;
                    }
                }
                temp_J = temp_J -> next;
            }
            temp = temp -> next;
        }
        customer* MinIndex = findMin(start, end, AnsElementCount);
        customer* tempMinIndex = MinIndex;
        do{
            tempMinIndex -> print();
            tempMinIndex = tempMinIndex -> next;
        }while(tempMinIndex != end -> next);
        while(start != MinIndex){
            start -> print();
            start = start -> next;
        }
    }
    void DOMAIN_EXPANSION()
    {
        //cout << "domain_expansion" << endl;
        if (GuestInTable == 0) return;
        int JujutsuSorcerers = 0; //! Số lượng Chú thuật sư
        int CursedSpirits = 0; //! Số lượng Oán linh
        int TotalSocererEnergy = 0, TotalSpiritEnergy = 0;
        //! Tách ra thành 2 danh sách chú thuật sư và oán linh
        customerOrder* JujutsuSorcererHead = nullptr;
        customerOrder* JujutsuSorcererMid = nullptr;
        customerOrder* JujutsuSorcererTail = nullptr;
        customerOrder* CursedSpiritsHead = nullptr;
        customerOrder* CursedSpiritsMid = nullptr;
        customerOrder* CursedSpiritsTail = nullptr;
        customerOrder* tempOrder = orderHead;
        for (int i = 0; i < GuestInTable; i++){
            if (tempOrder -> data -> energy > 0){
                AppendOrder(JujutsuSorcererHead, JujutsuSorcererTail, tempOrder -> data);
                TotalSocererEnergy += JujutsuSorcererTail -> data -> energy;
                JujutsuSorcerers += 1;
            }
            else {
                AppendOrder(CursedSpiritsHead, CursedSpiritsTail, tempOrder -> data);
                TotalSpiritEnergy += abs(CursedSpiritsTail -> data -> energy);
                CursedSpirits += 1;
            }
            tempOrder = tempOrder -> next;
        }
        JujutsuSorcererMid = JujutsuSorcererTail; CursedSpiritsMid = CursedSpiritsTail;
        customer* tempQueue = GuestQueue; //! Lấy luôn cả hàng chờ
        for (int i = 0; i < GuestInQueue; i++){
            if (tempQueue -> energy > 0){
                AppendOrder(JujutsuSorcererHead, JujutsuSorcererTail, tempQueue);
                JujutsuSorcererTail -> QueueCheck = true;
                TotalSocererEnergy += JujutsuSorcererTail -> data -> energy;
                JujutsuSorcerers += 1;
            }
            else{
                AppendOrder(CursedSpiritsHead, CursedSpiritsTail, tempQueue);
                CursedSpiritsTail -> QueueCheck = true;
                TotalSpiritEnergy += abs(CursedSpiritsTail -> data -> energy);
                CursedSpirits += 1;
            }
            tempQueue = tempQueue -> next;
        }
        //! Tổng ENERGY của tất cả chú thuật sư lớn hơn hoặc bằng tổng trị tuyệt đối ENERGY
        //của tất cả chú linh có mặt tại nhà hàng
        if (TotalSocererEnergy >= TotalSpiritEnergy){
            DeleteType(CursedSpiritsHead, CursedSpiritsTail, CursedSpirits);
        }
        else{
            DeleteType(JujutsuSorcererHead, JujutsuSorcererTail, JujutsuSorcerers);
        }
        ClearOrder(orderHead, orderTail); //Xóa thứ tự khách ban đầu
        if (JujutsuSorcerers){
            if (JujutsuSorcererMid != nullptr){
                orderHead = JujutsuSorcererHead;
                orderTail = JujutsuSorcererMid;
                ClearOrder(JujutsuSorcererMid -> next, JujutsuSorcererTail);
                JujutsuSorcererMid -> next = nullptr;
            }
            else ClearOrder(JujutsuSorcererHead, JujutsuSorcererTail);
            while (GuestInQueue != 0 && GuestInTable < MAXSIZE){
                customer* Guest = PopGuest();
                AppendGuestTable(Guest);
            }
        }
        else{
            if (CursedSpiritsMid != nullptr){
                orderHead = CursedSpiritsHead;
                orderTail = CursedSpiritsMid;
                ClearOrder(CursedSpiritsMid -> next, CursedSpiritsTail);
                CursedSpiritsMid -> next = nullptr;
            }
            else ClearOrder(CursedSpiritsHead, CursedSpiritsTail);
            while (GuestInQueue != 0 && GuestInTable < MAXSIZE){
                customer* Guest = PopGuest();
                AppendGuestTable(Guest);
            }
        }
        TableOrder = orderHead -> data;
    }
    void LIGHT(int num)
    {
        //cout << "light " << num << endl;
        if (num > 0){
            customer* temp = CustomerX;
            for (int i = 0; i < GuestInTable; i++) {
                temp->print();
                temp = temp->next;
            }
        }
        else if (num < 0){
            customer* temp = CustomerX;
            for (int i = 0; i < GuestInTable; i++){
                temp->print();
                temp = temp->prev;
            }
        }
        else{
            customer* temp = GuestQueue;
            for (int i = 0; i < GuestInQueue; i++){
                temp->print();
                temp = temp -> next;
            }
        }
    }
};

void imp_res::AppendOrder(customerOrder *& head, customerOrder *& tail, customer* Customer) {
    if (!head && !tail){
        head = tail = new customerOrder(Customer);
        COUNTDELETE -= 1;
        return;
    }
    tail -> next = new customerOrder(Customer);
    COUNTDELETE -= 1;
    tail -> next -> prev = tail;
    tail = tail -> next;
}

void imp_res::ClearOrder(customerOrder*& head, customerOrder*& tail) {
    while(head != nullptr){
        customerOrder * temp = head;
        head = head -> next;
        temp -> data = nullptr;
        delete temp;
        COUNTDELETE += 1;
    }
    head = tail = nullptr;
}

bool imp_res::CheckDuplicate(customer* cus) {
    customer* tempTable = TableOrder;
    customer* tempQueue = GuestQueue;
    for (int i = 0; i < GuestInTable; i++){
        if (tempTable -> name == cus -> name) return true;
        tempTable = tempTable -> next;
    }
    for (int i = 0; i < GuestInQueue; i++){
        if (tempQueue -> name == cus -> name) return true;
        tempQueue = tempQueue -> next;
    }
    return false;
}

void imp_res::AppendGuestTable(customer* cus) {
    if (TableOrder == nullptr){ //! Truờng hơợp bàn ăn trống
        TableOrder = cus;
        TableOrder -> next = TableOrder -> prev = TableOrder;
        GuestInTable = 1;
        CustomerX = TableOrder;
        AppendOrder(orderHead, orderTail, CustomerX);
        return;
    }
    //! Trường hợp bàn ăn không trống
    if (GuestInTable >= MAXSIZE / 2){
        customer* tempTable = CustomerX;
        int MaxDiff = -999999;
        for (int i = 0; i < GuestInTable; i++){
            if (abs(cus->energy - tempTable->energy) > MaxDiff){
                MaxDiff = abs(cus->energy - tempTable->energy);
                CustomerX = tempTable;
            }
            tempTable = tempTable -> next;
        }
    }
    if (cus->energy >= CustomerX->energy){
        cus -> next = CustomerX -> next;
        CustomerX -> next -> prev = cus;
        CustomerX -> next = cus;
        cus -> prev = CustomerX;
    }
    else{
        CustomerX -> prev -> next = cus;
        cus -> prev = CustomerX -> prev;
        cus -> next = CustomerX;
        CustomerX -> prev = cus;
    }
    CustomerX = cus;
    AppendOrder(orderHead, orderTail, CustomerX);
    GuestInTable += 1;
}

void imp_res::AppendGuestQueue(customer* cus) {
    if (GuestQueue == nullptr){
        GuestQueue = cus;
        GuestQueue -> next = GuestQueue -> prev = GuestQueue;
        GuestInQueue = 1;
        return;
    }
    GuestQueue -> prev -> next  = cus;
    cus -> prev = GuestQueue -> prev;
    cus -> next = GuestQueue;
    GuestQueue -> prev = cus;
    GuestInQueue += 1;
}

Restaurant::customer* imp_res::PopGuest() {
    customer* tempGuest = nullptr;
    if (GuestInQueue == 0) return nullptr;
    if (GuestInQueue == 1){
        tempGuest = GuestQueue;
        tempGuest -> next = tempGuest -> prev = nullptr;
        GuestQueue = nullptr;
        GuestInQueue = 0;
        return tempGuest;
    }
    tempGuest = GuestQueue;
    GuestQueue = GuestQueue -> next;
    tempGuest -> prev -> next = tempGuest -> next;
    tempGuest -> next -> prev = tempGuest -> prev;
    tempGuest -> next = tempGuest -> prev = nullptr;
    GuestInQueue -= 1;
    return tempGuest;
}

Restaurant::customer* imp_res::PopGuest(Restaurant::customer * cus) {
    customer* tempGuest = nullptr;
    if (GuestQueue == 0) return nullptr;
    if ((GuestInQueue == 1 && cus == GuestQueue) || cus == GuestQueue){
        tempGuest = PopGuest();
        return tempGuest;
    }
    cus -> prev -> next = cus -> next;
    cus -> next -> prev = cus -> prev;
    tempGuest = cus;
    tempGuest -> next = tempGuest -> prev = nullptr;
    GuestInQueue -= 1;
    return tempGuest;
}

void imp_res::RemoveGuestOrder(int num, customerOrder*& head, customerOrder*& tail) {
    //if (!head && !tail) return;
    for (int i = 0; i < num; i++){
        customerOrder* tempOrder = head;
        head = head -> next;
        if (tempOrder -> data == TableOrder && tempOrder -> next != nullptr) TableOrder = tempOrder -> next -> data;
        if (tempOrder -> data -> energy > 0) CustomerX = tempOrder -> data -> next;
        else CustomerX = tempOrder -> data -> prev;
        tempOrder -> data -> prev -> next = tempOrder -> data -> next;
        tempOrder -> data -> next -> prev = tempOrder -> data -> prev;
        tempOrder -> data -> next = tempOrder -> data -> prev = nullptr;
        tempOrder -> next = tempOrder -> prev = nullptr;
        delete tempOrder;
        COUNTDELETE += 1;
        GuestInTable -= 1;
        if (GuestInTable == 0){
            CustomerX = TableOrder = nullptr;
            ClearOrder(orderHead, orderTail); //! Phòng trường hợp bất trắc
        }
    }
}

void imp_res::DeleteType(customerOrder *&head, customerOrder *&tail, int &length) {
    customerOrder* temp = tail;
    for (int i = 0; i < length; i++){
        temp -> data -> print();
        temp = temp -> prev;
    }
    for (int i = 0; i < length; i++){
        if (tail != nullptr && tail -> QueueCheck){
            temp = tail;
            tail = tail -> prev;
            PopGuest(temp -> data);
            temp -> next = temp -> prev = nullptr;
            delete temp;
            //tail -> next = nullptr;
            COUNTDELETE += 1;
        }
        else{
            customerOrder* tempOrder = tail;
            tail = tail -> prev;
            if (tempOrder -> data == TableOrder && tempOrder -> prev != nullptr) TableOrder = tempOrder -> prev -> data;
            if (tempOrder -> data -> energy > 0) CustomerX = tempOrder -> data -> next;
            else CustomerX = tempOrder -> data -> prev;
            tempOrder -> data -> prev -> next = tempOrder -> data -> next;
            tempOrder -> data -> next -> prev = tempOrder -> data -> prev;
            delete tempOrder;
            //tail -> next = nullptr;
            COUNTDELETE += 1;
            GuestInTable -= 1;
            if (GuestInTable == 0){
                CustomerX = TableOrder = nullptr;
                ClearOrder(orderHead, orderTail); //! Phòng trường hợp bất trắc
            }
        }
    }
//    for (int i = 0; i < length; i++){
//        if (head != nullptr && head -> QueueCheck){
//            temp = head;
//            head = head -> next;
//            PopGuest(temp -> data);
//            temp -> next = temp -> prev = nullptr;
//            delete temp;
//            COUNTDELETE += 1;
//        }
//        else{
//            RemoveGuestOrder(1, head, tail);
//        }
//    }
    length = 0;
}

Restaurant::customer* imp_res::findMin(Restaurant::customer *start, Restaurant::customer *end, int length) {
    int min_val = INT_MAX;
    customer* MinIndex = nullptr;
    for (int i = 0; i < length; i++){
        if (min_val > start -> energy){
            MinIndex = start;
            min_val = start -> energy;
        }
        start = start -> next;
    }
    return MinIndex;
}

void imp_res::SwapNode(Restaurant::customer *& Cus1, Restaurant::customer *& Cus2) {
    if (TableOrder == Cus1) TableOrder = Cus2;
    else if (TableOrder == Cus2) TableOrder = Cus1;
    customer * temp = Cus1 -> next;
    Cus1 -> next = Cus2 -> next;
    Cus2 -> next = temp;
    Cus1 -> next -> prev = Cus1;
    Cus2 -> next -> prev = Cus2;
    temp = Cus1 -> prev;
    Cus1 -> prev = Cus2 -> prev;
    Cus2 -> prev = temp;
    Cus1 -> prev -> next = Cus1;
    Cus2 -> prev -> next = Cus2;
    temp = Cus1;
    Cus1 = Cus2;
    Cus2 = temp;
}