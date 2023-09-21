#include <sstream>
#include "json.hpp"

struct ListCell{
    json info;
    ListCell* next;
};
typedef ListCell* Cell_L;

struct DictCell{
    std::pair<std::string, json> info;
    DictCell* next;
};
typedef DictCell* Cell_D;

struct json::impl{
    int type;
    std::string j_string;
    double number;
    bool boolean;
    Cell_L list_head;
    Cell_L list_tail;
    Cell_D dict_head;
    Cell_D dict_tail;

    void list_append(json const& info);
    void dict_append(std::pair<std::string, json> const& info);
    void list_delete();
    void dict_delete();
};

struct json::list_iterator{
    list_iterator(Cell_L ptr);
    json& operator*();
    json* operator->();
    list_iterator& operator++();
    list_iterator operator++(int);
    bool operator==(list_iterator const& rhs) const;
    bool operator!=(list_iterator const& rhs) const;
    Cell_L l_ptr;
};

json::list_iterator::list_iterator(Cell_L ptr){
    l_ptr=ptr;
}

json& json::list_iterator::operator*(){
    return l_ptr->info;
}

json* json::list_iterator::operator->(){
    return &(l_ptr->info);
}

json::list_iterator& json::list_iterator::operator++(){
    l_ptr = l_ptr->next;
    return *this;
}

json::list_iterator json::list_iterator::operator++(int){
    list_iterator temp = {l_ptr};
    ++(*this);
    return temp;
}

bool json::list_iterator::operator==(list_iterator const& rhs) const{
    return l_ptr==rhs.l_ptr;
}

bool json::list_iterator::operator!=(list_iterator const& rhs) const{
    return l_ptr!=rhs.l_ptr;
}

json::list_iterator json::begin_list(){
    //std::cout<<"Sono dentro il begin_list()"<<std::endl;
    if (!is_list()) {
        //std::cout<<"sono a riga 75"<<std::endl;
        throw json_exception{"L'oggetto non e' una lista"};
    }

    else {
        return {pimpl->list_head};
    }
}

json::list_iterator json::end_list(){

    if (!is_list()) {
        //std::cout<<"sono a riga 83"<<std::endl;
        throw json_exception{"L'oggetto non e' una lista"};
    }
    else{
        return {nullptr};
    }
}


struct json::const_list_iterator{
    const_list_iterator(Cell_L ptr);
    json const& operator*() const;
    const json* operator->() const;
    const_list_iterator& operator++();
    const_list_iterator operator++(int);
    bool operator==(const_list_iterator const& rhs) const;
    bool operator!=(const_list_iterator const& rhs) const;
    Cell_L lc_ptr;
};

json::const_list_iterator::const_list_iterator(Cell_L ptr){
    lc_ptr=ptr;
}

json const& json::const_list_iterator::operator*() const{
    //std::cout<<"Sono dentro l'operator * const"<<std::endl;
    //std::cout<<"lc_ptr->info="<<lc_ptr->info<<std::endl;
    return lc_ptr->info;
}

const json* json::const_list_iterator::operator->() const{
    return &(lc_ptr->info);
}

json::const_list_iterator& json::const_list_iterator::operator++(){
    lc_ptr = lc_ptr->next;
    return *this;
}

json::const_list_iterator json::const_list_iterator::operator++(int){
    const_list_iterator temp = {lc_ptr};
    ++(*this);
    return temp;
}

bool json::const_list_iterator::operator==(const_list_iterator const& rhs) const{
    return lc_ptr==rhs.lc_ptr;
}

bool json::const_list_iterator::operator!=(const_list_iterator const& rhs) const{
    return lc_ptr!=rhs.lc_ptr;
}

json::const_list_iterator json::begin_list() const{
    //std::cout<<"Sono dentro il begin_list() const "<<std::endl;
    //std::cout<<"dentro begin_list const, is_list="<<is_list()<<std::endl;
    //std::cout<<"is_dictionary="<<is_dictionary()<<std::endl;
    if (!is_list()) {
        //std::cout<<"sono a riga 135"<<std::endl;
        throw json_exception{"L'oggetto non e' una lista"};
    }
    else {
        return {pimpl->list_head};
    }
}

json::const_list_iterator json::end_list() const{
    if (!is_list()) {
        //std::cout<<"sono a riga 143"<<std::endl;
        throw json_exception{"L'oggetto non e' una lista"};
    }
    else{
        return {nullptr};
    }
}


struct json::dictionary_iterator{
    dictionary_iterator(Cell_D ptr);
    std::pair<std::string, json>& operator*();
    std::pair<std::string, json>* operator->();
    dictionary_iterator& operator++();
    dictionary_iterator operator++(int);
    bool operator==(dictionary_iterator const& rhs) const;
    bool operator!=(dictionary_iterator const& rhs) const;
    Cell_D d_ptr;
};

json::dictionary_iterator::dictionary_iterator(Cell_D ptr){
    d_ptr=ptr;
}

std::pair<std::string, json>& json::dictionary_iterator::operator*(){
    return d_ptr->info;
}

std::pair<std::string, json>* json::dictionary_iterator::operator->(){
    return &(d_ptr->info);
}

json::dictionary_iterator& json::dictionary_iterator::operator++(){
    d_ptr = d_ptr->next;
    return *this;
}

json::dictionary_iterator json::dictionary_iterator::operator++(int){
    dictionary_iterator temp = {d_ptr};
    ++(*this);
    return temp;
}

bool json::dictionary_iterator::operator==(dictionary_iterator const& rhs) const{
    return d_ptr==rhs.d_ptr;
}

bool json::dictionary_iterator::operator!=(dictionary_iterator const& rhs) const{
    return d_ptr!=rhs.d_ptr;
}

json::dictionary_iterator json::begin_dictionary(){

    if (!is_dictionary()) {
        //std::cout<<"sono a riga 194"<<std::endl;
        throw json_exception{"L'oggetto non e' un dizionario"};
    }
    else {
        return {pimpl->dict_head};
    }
}

json::dictionary_iterator json::end_dictionary(){

    if (!is_dictionary()) {
        //std::cout<<"sono a riga 202"<<std::endl;
        throw json_exception{"L'oggetto non e' un dizionario"};
    }
    else{
        return {nullptr};
    }
}


struct json::const_dictionary_iterator{
    const_dictionary_iterator(Cell_D ptr);
    std::pair<std::string, json> const& operator*() const;
    const std::pair<std::string, json>* operator->() const;
    const_dictionary_iterator& operator++();
    const_dictionary_iterator operator++(int);
    bool operator==(const_dictionary_iterator const& rhs) const;
    bool operator!=(const_dictionary_iterator const& rhs) const;
    Cell_D dc_ptr;
};

json::const_dictionary_iterator::const_dictionary_iterator(Cell_D ptr){
    dc_ptr=ptr;
}

std::pair<std::string, json> const& json::const_dictionary_iterator::operator*() const{
    return dc_ptr->info;
}

const std::pair<std::string, json>* json::const_dictionary_iterator::operator->() const{
    return &(dc_ptr->info);
}

json::const_dictionary_iterator& json::const_dictionary_iterator::operator++(){
    dc_ptr = dc_ptr->next;
    return *this;
}

json::const_dictionary_iterator json::const_dictionary_iterator::operator++(int){
    const_dictionary_iterator temp = {dc_ptr};
    ++(*this);
    return temp;
}

bool json::const_dictionary_iterator::operator==(const_dictionary_iterator const& rhs) const{
    return dc_ptr==rhs.dc_ptr;
}

bool json::const_dictionary_iterator::operator!=(const_dictionary_iterator const& rhs) const{
    return dc_ptr!=rhs.dc_ptr;
}

json::const_dictionary_iterator json::begin_dictionary() const{
    if (!is_dictionary()) {
        //std::cout<<"sono a riga 252"<<std::endl;
        throw json_exception{"L'oggetto non e' un dizionario"};
    }
    else {
        return {pimpl->dict_head};
    }
}

json::const_dictionary_iterator json::end_dictionary() const{

    if (!is_dictionary()) {
        //std::cout<<"sono a riga 260"<<std::endl;
        throw json_exception{"L'oggetto non e' un dizionario"};
    }
    else{
        return {nullptr};
    }
}

void json::impl::list_append(json const& source) {
    //std::cout<<"Sono nella list_append e source.pimpl->num="<<source.pimpl->number<<std::endl;
    Cell_L newCell = new ListCell;
    newCell->info.pimpl->type=source.pimpl->type;
    newCell->info = source;
    newCell->next = nullptr;
    if (!list_head) {
        list_head=newCell;
        list_tail=newCell;
    }
    else {
        list_tail->next = newCell;
        list_tail = newCell;
    }
}

void json::impl::list_delete() {
    Cell_L app_L;
    while(list_head) {
        app_L = list_head;
        list_head=list_head->next;
        delete app_L;
    }
    list_head = nullptr;
    list_tail = nullptr;
}

void json::impl::dict_append(std::pair<std::string, json> const& source) {
    //std::cout<<"Sono dentro la dict_append"<<std::endl;
    Cell_D newCell = new DictCell;
    newCell->info.second.pimpl->type=source.second.pimpl->type;
    newCell->info = source;
    newCell->next = nullptr;
    if (!dict_head) {
        dict_head=newCell;
        dict_tail=newCell;
    }
    else {
        dict_tail->next = newCell;
        dict_tail = newCell;
    }
    //std::cout<<"Sono alla fine della dict_append"<<std::endl;
}

void json::impl::dict_delete() {
    Cell_D app_D;
    while(dict_head) {
        app_D = dict_head;
        dict_head=dict_head->next;
        delete app_D;
    }
    dict_head = nullptr;
    dict_tail = nullptr;
}

json::json(){
    //std::cout<<"Sono nel costruttore di default"<<std::endl;
    pimpl=new impl;
    pimpl->type=0;
    pimpl->list_head = nullptr;
    pimpl->list_tail = nullptr;
    pimpl->dict_head = nullptr;
    pimpl->dict_tail = nullptr;
}

json::json(const json & source) {
    //std::cout<<"Sono nel costruttore per copia"<<std::endl;
    pimpl=new impl;
    pimpl->type = source.pimpl->type;
    pimpl->j_string = source.pimpl->j_string;
    pimpl->number = source.pimpl->number;
    pimpl->boolean = source.pimpl->boolean;
    pimpl->list_head = nullptr;
    pimpl->list_tail = nullptr;
    pimpl->dict_head = nullptr;
    pimpl->dict_tail = nullptr;

    Cell_L list_ptr = source.pimpl->list_head;
    while (list_ptr){
        pimpl->list_append(list_ptr->info);
        list_ptr=list_ptr->next;
    }
    Cell_D dict_ptr = source.pimpl->dict_head;
    while (dict_ptr){
        pimpl->dict_append(dict_ptr->info);
        dict_ptr=dict_ptr->next;
    }
}

json::~json() {
    //std::cout<<"Sono nel distruttore"<<std::endl;
    pimpl->list_delete();
    pimpl->dict_delete();
    delete pimpl;
}

json::json(json&& source){
    //std::cout<<"Sono nel move constructor"<<std::endl;
    this->set_null();
    *this=std::move(source);
}

bool json::is_null() const{
    return pimpl->type==0;
}

bool json::is_string() const{
    return pimpl->type==1;
}

bool json::is_number() const{
    return pimpl->type==2;
}

bool json::is_bool() const{
    return pimpl->type==3;
}

bool json::is_list() const{
    return pimpl->type==4;
}

bool json::is_dictionary() const{
    return pimpl->type==5;
}

json& json::operator=(json const& rhs){
    //std::cout<<"sono dentro l'operator ="<<std::endl;
    if (this != &rhs){
        if(is_list()){
            pimpl->list_delete();
            for (const_list_iterator it = rhs.begin_list(); it != rhs.end_list(); ++it){
                pimpl->list_append(it.lc_ptr->info);
            }
        }
        if(is_dictionary()){
            pimpl->dict_delete();
            for (const_dictionary_iterator it = rhs.begin_dictionary(); it != rhs.end_dictionary(); ++it){
                pimpl->dict_append(it.dc_ptr->info);
            }
        }
        pimpl->type = rhs.pimpl->type;
        pimpl->j_string = rhs.pimpl->j_string;
        pimpl->number = rhs.pimpl->number;
        pimpl->boolean = rhs.pimpl->boolean;
    }
    return *this;
}

json& json::operator=(json&& rhs) {
    //std::cout<<"sono dentro il move assignment"<<std::endl;
    if (this != &rhs) {
        pimpl->list_delete();
        pimpl->dict_delete();
        pimpl->list_head = rhs.pimpl->list_head;
        pimpl->list_tail = rhs.pimpl->list_tail;
        pimpl->dict_head = rhs.pimpl->dict_head;
        pimpl->dict_tail = rhs.pimpl->dict_tail;
        rhs.pimpl->list_head = rhs.pimpl->list_tail = nullptr;
        rhs.pimpl->dict_head = rhs.pimpl->dict_tail = nullptr;
        pimpl->type = rhs.pimpl->type;
        pimpl->j_string = rhs.pimpl->j_string;
        pimpl->number = rhs.pimpl->number;
        pimpl->boolean = rhs.pimpl->boolean;
    }
    return *this;
}


json& json::operator[](std::string const& key){
    if (is_dictionary()){
        dictionary_iterator it = begin_dictionary();
        while (it != end_dictionary()){
            if (it.d_ptr->info.first == key){
                return it.d_ptr->info.second;
            }
            it++;
        }
        json jsonNull;
        std::pair<std::string,json> newEl;
        newEl.first = key;
        newEl.second = jsonNull;
        pimpl->dict_append(newEl);
        return pimpl->dict_tail->info.second;
    }

    else{
        //std::cout<<"sono a riga 466"<<std::endl;
        throw json_exception{"Errore. L'oggetto sul quale viene chiamato l'operatore [] non e' un dizionario"};
    }
}

json const& json::operator[](std::string const& key) const{
    if (is_dictionary()){
        const_dictionary_iterator it = begin_dictionary();
        while (it != end_dictionary()){
            if (it.dc_ptr->info.first == key){
                return it.dc_ptr->info.second;
            }
            it++;
        }
        throw json_exception{"Errore. Non è possibile fare un inserimento nel dizionario tramite l'operatore [] const"};
    }

    else {
        //std::cout<<"sono a riga 492"<<std::endl;
        throw json_exception{"Errore. L'oggetto sul quale viene chiamato l'operatore [] const non e' un dizionario"};
    }
}


double& json::get_number(){
    if (is_number()) {return pimpl->number;}
    else {
        //std::cout<<"sono a riga 499"<<std::endl;
        throw json_exception{"Errore. L'oggetto sul quale viene chiamata la funzione non e' un numero"};
    }
}

double const& json::get_number() const{
    if (is_number()) {return pimpl->number;}
    else {
        //std::cout<<"sono a riga 505"<<std::endl;
        throw json_exception{"Errore. L'oggetto sul quale viene chiamata la funzione non e' un numero"};
    }
}

bool& json::get_bool() {
    if (is_bool()) {return pimpl->boolean;}
    else {
        //std::cout<<"sono a riga 510"<<std::endl;
        throw json_exception{"Errore. L'oggetto sul quale viene chiamata la funzione non e' un booleano"};
    }
}

bool const& json::get_bool() const{
    if (is_bool()) {return pimpl->boolean;
    }
    else {
        //std::cout<<"sono a riga 516"<<std::endl;
        throw json_exception{"Errore. L'oggetto sul quale viene chiamata la funzione non e' un booleano"};
    }
}

std::string& json::get_string(){
    if (is_string()) {
        return pimpl->j_string;
    }
    else {
        //std::cout<<"sono a riga 525"<<std::endl;
        throw json_exception{"Errore. L'oggetto sul quale viene chiamata la funzione non e' una stringa"
        };
    }

}

std::string const& json::get_string() const{
    if (is_string()) {
        return pimpl->j_string;
    }
    else {
        //std::cout << "Sono a riga 531" << std::endl;
        throw json_exception{"Errore. L'oggetto sul quale viene chiamata la funzione non e' una stringa"};
    }
}

void json::set_null(){
    pimpl->list_delete();
    pimpl->dict_delete();
    pimpl->type=0;
}

void json::set_string(std::string const& source_string){
    pimpl->list_delete();
    pimpl->dict_delete();
    pimpl->type=1;
    pimpl->j_string=source_string;
}

void json::set_number(double source_double){
    pimpl->list_delete();
    pimpl->dict_delete();
    pimpl->type=2;
    pimpl->number=source_double;
}

void json::set_bool(bool source_bool){
    pimpl->list_delete();
    pimpl->dict_delete();
    pimpl->type=3;
    pimpl->boolean= source_bool;
}

void json::set_list(){
    pimpl->list_delete();
    pimpl->dict_delete();
    pimpl->type=4;
}

void json::set_dictionary(){
    pimpl->list_delete();
    pimpl->dict_delete();
    pimpl->type=5;
    //std::cout<<"dentro la set_dict pimpl->type= "<<pimpl->type<<std::endl;
}

void json::push_front(json const& source_json){
    if (pimpl->type!=4) {
        //std::cout<<"sono a riga 577"<<std::endl;
        throw json_exception{"Errore. L'oggetto sul quale viene chiamata la funzione non e' una lista"};
    }
    Cell_L newCell = new ListCell;
    newCell->info = source_json;
    newCell->next = pimpl->list_head;
    pimpl->list_head = newCell;
    if (pimpl->list_tail==nullptr)
        pimpl->list_tail=newCell;
}

void json::push_back(json const& source_json){
    //std::cout<<"sono nella push_back e pimpl->type="<<pimpl->type<<std::endl;
    if (pimpl->type!=4) {
        //std::cout<<"sono a riga 588"<<std::endl;
        throw json_exception{"Errore. L'oggetto sul quale viene chiamata la funzione non e' una lista"};
    }
    pimpl->list_append(source_json);
}

void json::insert(std::pair<std::string, json> const& source_pair){
    //std::cout<<"sono dentro la insert e pimpl->type="<<pimpl->type<<std::endl;
    if (pimpl->type!=5) {
        //std::cout<<"sono a riga 594"<<std::endl;
        throw json_exception{"Errore. L'oggetto sul quale viene chiamata la funzione non e' un dizionario"};
    }
    pimpl->dict_append(source_pair);
}

std::ostream& operator<<(std::ostream& lhs, json const& rhs){
    //std::cout<<"Sono al passo 3"<<std::endl;
    if (rhs.is_null()){
        lhs<<"null";
    }
    else if (rhs.is_bool()){
        //std::cout<<"Sono dentro al caso per la stampa is_bool"<<std::endl;
        if(rhs.get_bool()==1) lhs<<"true";
        else lhs<<"false";
    }
    else if (rhs.is_number()){
        //std::cout<<"Sono dentro al caso per la stampa is_number"<<std::endl;
        lhs<<rhs.get_number();
    }
    else if (rhs.is_string()){
        //std::cout<<"Sono dentro al caso per la stampa is_string"<<std::endl;
        lhs<<'"';
        lhs<<rhs.get_string();
        lhs<<'"';
    }
    else if (rhs.is_list()){
        //std::cout<<"Sono dentro al caso per la stampa is_list"<<std::endl;
        lhs<<'[';
        for (json::const_list_iterator it = rhs.begin_list(); it != rhs.end_list();){
            lhs<<(*it);
            ++it;
            if (it != rhs.end_list()) lhs<<',';
        }
        lhs<<']';
    }
    else if (rhs.is_dictionary()){
        //std::cout<<"Sono dentro al caso per la stampa is_dictionary"<<std::endl;
        lhs<<'{';
        for (json::const_dictionary_iterator it = rhs.begin_dictionary(); it != rhs.end_dictionary();){
            lhs<<'"';
            lhs<<(*it).first;
            lhs<<'"';
            lhs<<':';
            lhs<<(*it).second;
            ++it;
            if (it != rhs.end_dictionary()) lhs<<',';
        }
        lhs<<'}';
    }
    return lhs;
}


json L(std::istream& lhs);
json D(std::istream& lhs);

json J(std::istream& lhs){
    if (lhs.eof()) { // se lo stream è vuoto
        //std::cout<<"sono a riga 755"<<std::endl;
        throw json_exception{"Errore nel file json dato in input"};
    }
    //std::cout<<"sono dentro J"<<std::endl;
    json newJson;
    char nextChar;
    lhs>>nextChar;
    //std::cout<<"nextChar="<<nextChar<<std::endl;
    if(nextChar == '['){ // caso lista
        if (lhs.eof()) {
            //std::cout<<"sono a riga 766"<<std::endl;
            throw json_exception{"Errore nel file json dato in input"};
        }
        //std::cout<<"sono dentro il caso '['"<<std::endl;
        newJson.set_list();
        newJson=L(lhs);
        lhs>>nextChar; // leggo ']'
        if (nextChar!=']') {
            //std::cout<<"sono a riga 773"<<std::endl;
            throw json_exception{"Errore nel file json dato in input"};
        }
        //std::cout<<"nextChar dovrebbe essere ']' ed e'="<<nextChar<<std::endl;
    }
    else if (nextChar == '"'){ // caso stringa
        if (lhs.eof()) {
            //std::cout<<"sono a riga 779"<<std::endl;
            throw json_exception{"Errore nel file json dato in input"};
        }
        //std::cout<<"sono dentro il caso '\"'"<<std::endl;
        std::string newString ="";
        nextChar=lhs.get();
        while(nextChar != '\"' && !lhs.eof()){
            //std::cout<<"sono dentro il while"<<std::endl;
            if (nextChar=='\\') {
                newString += nextChar;
                nextChar=lhs.get();
            }
            newString += nextChar;
            nextChar=lhs.get();
        }
        if (lhs.eof()) {
            //std::cout<<"sono a riga 797"<<std::endl;
            throw json_exception{"Errore nel file json dato in input"};
        }
        newJson.set_string(newString);
        //std::cout<<"newString="<<newString<<std::endl;
    }
    else if (nextChar == '{'){ // caso dizionario
        if (lhs.eof()) {
            //std::cout<<"sono a riga 805"<<std::endl;
            throw json_exception{"Errore nel file json dato in input"};
        }
        //std::cout<<"Sono entrato nel caso '{'"<<std::endl;
        newJson.set_dictionary();
        newJson = D(lhs);
        lhs>>nextChar; // leggo '}'
        if (nextChar!='}') {
            //std::cout<<"sono a riga 813"<<std::endl;
            throw json_exception{"Errore nel file json dato in input"};
        }
        //std::cout<<"nextChar dovrebbe essere '}' ed e'="<<nextChar<<std::endl;
    }
    else if (nextChar == '-' || nextChar == '.' || (nextChar >=48 && nextChar <=57)){ // caso double
        //std::cout<<"sono dentro il caso numero"<<std::endl;
        double newDouble = 0;
        std::string numberString = "";
        bool negative =false;
        if (nextChar=='-') {
            negative = true;
            if (lhs.eof()) {
                //std::cout<<"sono a riga 747"<<std::endl;
                throw json_exception{"Errore nel file json dato in input"};
            }
            lhs>>nextChar;
        }
        while (nextChar >=48 && nextChar <=57 && !lhs.eof()){
            numberString+=nextChar;
            lhs>>nextChar;
        }
        double mult = 1;
        if (negative){
            for (int i=numberString.length(); i>0; i--){
                newDouble -= (numberString.at(i-1)-48) * mult;
                mult*=10;
            }
        }
        else{
            for (int i=numberString.length(); i>0; i--){
                newDouble += (numberString.at(i-1)-48) * mult;
                mult*=10;
            }
        }

        double decimals=0;
        if (nextChar == '.') {
            if (lhs.eof()) {
                //std::cout<<"sono a riga 774"<<std::endl;
                throw json_exception{"Errore nel file json dato in input"};
            }
            double i = 10.0;
            lhs>>nextChar;
            if (nextChar<48 || nextChar>57) {
                //std::cout<<"sono a riga 851"<<std::endl;
                throw json_exception{"Errore nel file json dato in input"};
            }
            while (nextChar >=48 && nextChar <=57 && !lhs.eof()){
                decimals += (double)((nextChar-48)/i);
                i *= 10;
                lhs>>nextChar;
            }
        }
        //std::cout<<",="<<nextChar<<std::endl;
        if (negative){
            newDouble-=decimals;
        }
        else{
            newDouble+=decimals;
        }
        //std::cout<<"newDouble="<<newDouble<<std::endl;
        newJson.set_number(newDouble);
        lhs.putback(nextChar);
    }
    else if (nextChar == 't' || nextChar == 'f'){ // caso boolean
        //std::cout<<"sono dentro il caso boolean"<<std::endl;
        bool correct = true;
        if (nextChar=='t'){
            lhs.putback('t');
            std::string nullString = "true";
            for (int i =0; i<4 && correct; ++i){
                lhs>>nextChar;
                if (nextChar != nullString.at(i)) correct = false;
            }

            if (!correct) {
                //std::cout<<"sono a riga 813"<<std::endl;
                throw json_exception{"Errore nel file json dato in input"};
            }
            else newJson.set_bool(true);
        }
        else {
            lhs.putback('f');
            std::string nullString = "false";
            for (int i =0; i<5; ++i){
                lhs>>nextChar;
                if (nextChar != nullString.at(i)) correct = false;
            }

            if (!correct) {
                //std::cout<<"sono a riga 827"<<std::endl;
                throw json_exception{"Errore nel file json dato in input"};
            }
            else newJson.set_bool(false);
        }
        //std::cout<<"nextChar dopo la set_bool="<<nextChar<<std::endl;
    }
    else if (nextChar == 'n'){ // caso null
        lhs.putback('n');
        std::string nullString = "null";
        bool correct = true;
        for (int i =0; i<4; ++i){
            lhs>>nextChar;
            if (nextChar != nullString.at(i)) correct = false;
        }
        if (!correct) {
            //std::cout<<"sono a riga 843"<<std::endl;
            throw json_exception{"Errore nel file json dato in input"};
        }
        else newJson.set_null();
    }
    else { // altrimenti
        //std::cout<<"sono a riga 849"<<std::endl;
        throw json_exception{"Errore nel file json dato in input"};
    }
    return newJson;
}

std::string D_string(std::istream& lhs){
    //std::cout<<"sono dentro D_string"<<std::endl;
    char nextChar;
    lhs>>nextChar; // leggo '"'
    if (nextChar!='\"') {
        //std::cout<<"sono a riga 898"<<std::endl;
        throw json_exception{"Errore nel file json dato in input"};
    }
    //std::cout<<"\"="<<nextChar<<std::endl;
    std::string newString="";
    nextChar=lhs.get();
    while(nextChar != '\"' && !lhs.eof()){ // finché l'input ha caratteri e il prossimo carattere è diverso da "
        if (nextChar=='\\') {
            newString += nextChar;
            nextChar=lhs.get();
        }
        newString += nextChar;
        nextChar=lhs.get();
    }
    if (nextChar!='\"' || lhs.eof()) {
        //std::cout<<"sono a riga 913"<<std::endl;
        throw json_exception{"Errore nel file json dato in input"};
    }
    //std::cout<<"newString="<<newString<<std::endl;
    //std::cout<<"torno da D_string"<<std::endl;
    return newString;
}

json D(std::istream& lhs){
    //std::cout<<"sono dentro D"<<std::endl;
    json newJson;
    newJson.set_dictionary();
    char nextChar;
    lhs>>nextChar;
    if (nextChar!='}'){
        lhs.putback(nextChar);
        do {
            std::pair<std::string, json> newPair;
            newPair.first= D_string(lhs);
            //std::cout<<"newPair.first="<<newPair.first<<std::endl;
            lhs>>nextChar; // leggo ':'
            if (nextChar!=':') {
                //std::cout<<"sono a riga 935"<<std::endl;
                throw json_exception{"Errore nel file json dato in input"};
            }
            //std::cout<<":="<<nextChar<<std::endl;
            newPair.second= J(lhs);
            newJson.insert(newPair);
            if (lhs.eof()){
                throw json_exception{"Errore nel file json dato in input"};
            }
            lhs>>nextChar; // leggo ','
        } while (nextChar==',');
    }
    lhs.putback(nextChar);
    return newJson;
}

json L(std::istream& lhs){
    //std::cout<<"sono dentro L"<<std::endl;
    json newJson;
    newJson.set_list();
    char nextChar;
    lhs>>nextChar;
    if (nextChar!=']'){
        lhs.putback(nextChar);
        newJson.push_back(J(lhs));
        lhs>>nextChar;
        while (nextChar == ','){ // finché leggo virgole
            //std::cout<<"sono dentro il while della lista e nextChar="<<nextChar<<std::endl;
            newJson.push_back(J(lhs));
            if (lhs.eof()){
                throw json_exception{"Errore nel file json dato in input"};
            }
            lhs>>nextChar;
        }
    }
    lhs.putback(nextChar);
    return newJson;
}

// Grammatica

// J -> [L] | {D} | "string" | double | bool | null
// L -> J | L,L
// D -> "string":J | D,D

std::istream& operator>>(std::istream& lhs, json& rhs){
    rhs = J(lhs);
    char nextChar;
    lhs>>nextChar;
    if (!lhs.eof()) {
        //std::cout<<"sono a riga 988"<<std::endl;
        throw json_exception{"Errore nel file json dato in input"};
    }
    return lhs;
}


int main() {
    /*
    json j;
    std::cin>>j;
    std::cout<<j<<std::endl;
*/

    std::string newFile = "";
    for (int i=1; i<102;++i){
        newFile = "C:\\Users\\ilmio\\Desktop\\Progetto_Pel_2023\\json_test\\"+std::to_string(i);
        newFile += ".txt";
        std::cout<<""<<std::endl;
        std::cout<<""<<std::endl;
        std::cout<<"Apro il file "<<i<<std::endl;
        std::ifstream file(newFile);
        if (!file.is_open()) {
            std::cout << "Failed to open the file." << std::endl;
            return 1;
        }
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();

        json data;

        ss >> data;
        std::cout << data << std::endl;
    }



/*
    std::ifstream file("C:\\Users\\ilmio\\Desktop\\Progetto_PeL_2023\\prova_json.txt");
    if (!file.is_open()) {
        //std::cout << "Failed to open the file." << std::endl;
        return 1;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    json data;

    ss >> data;
    std::cout << data << std::endl;

    //std::cout<<"Il parsing e' andato a buon fine"<<std::endl;

    return 0;
    */
}