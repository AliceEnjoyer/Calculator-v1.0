#include <QApplication>
#include <QtWidgets>

/*class buttonDelegate : public QStyledItemDelegate {
public:
    buttonDelegate(QObject *obj = NULL) : QStyledItemDelegate(obj) {}
    void paint(QPainter* painter, const QStyleOptionButton& option) override{

    }
};*/

class calculator : public QWidget{
public:
    QPushButton *b1 = new QPushButton("1", this), *b2 = new QPushButton("2", this), *b3 = new QPushButton("3", this), *b4 = new QPushButton("4", this),
                *b5 = new QPushButton("5", this), *b6 = new QPushButton("6", this), *b7 = new QPushButton("7", this), *b8 = new QPushButton("8", this),
                *b9 = new QPushButton("9", this), *b0 = new QPushButton("0", this), *bPlus = new QPushButton("+", this), *bMinus = new QPushButton("-", this),
                *bMulti = new QPushButton("*", this), *bDivide = new QPushButton("/", this), *bClear = new QPushButton("Clear", this),
                *res = new QPushButton("=", this), *bDot = new QPushButton(".", this), *bBraket0 = new QPushButton("(", this), *bBraket = new QPushButton(")", this)
                //*settings = new QPushButton("&settings", this)
                ;
    QLineEdit *showLine = new QLineEdit(this);

protected:

 QVBoxLayout *vbl = new QVBoxLayout(this);
 QHBoxLayout *hbl = new QHBoxLayout(this);
 QGridLayout *grid = new QGridLayout(this);
 QHBoxLayout *settings_hbl = new QHBoxLayout(this);
 int brakets = 0;
 bool zero_clk = false;
 QSignalMapper *sm = new QSignalMapper(this);

 double simple_count(QString txt, bool is_recursion, bool is_recursion_with_braket_on_borders){
      if(is_recursion_with_braket_on_borders){
          txt[0] = 'X';
          txt[txt.size() - 1] = 'X';
          txt.remove('X');
      }
      QVector<QString> str_phars;
      QVector<QChar> symbols;
      QVector<double> int_vars;
      QString buffer;
      QChar x;
      int count_brakets = 0;
      int count_brakets0 = 0;
      int count_main_brakets = 0;
      bool braket_is_cheking = false;
      bool is_multi_chek = false;

      if(is_recursion && !is_recursion_with_braket_on_borders){
           for(int i = 0; i < txt.size(); i++ ){
                x = txt[i];
                if(i == txt.size() - 1) {
                    buffer.push_back(x);
                    str_phars.push_back(buffer);
                    buffer.clear();
                    if(x == ')'){
                        count_brakets++;
                    }
                } else if (x == ')') {
                    count_brakets++;
                    buffer.push_back(x);
                } else if(x == '('){
                    braket_is_cheking = true;
                    count_brakets0++;
                    buffer.push_back(x);
                } else if(x != '*' && x != '/' && !braket_is_cheking){
                    buffer.push_back(x);
                } else if(x == '*' && !braket_is_cheking){
                    str_phars.push_back(buffer);
                    buffer.clear();
                    symbols.push_back('*');
                } else if(x == '/' && !braket_is_cheking){
                    str_phars.push_back(buffer);
                    buffer.clear();
                    symbols.push_back('/');
                } else {
                    buffer.push_back(x);
                }
                if(x == ')' && count_brakets == count_brakets0) {
                    count_brakets = 0;
                    braket_is_cheking = false;
                    count_main_brakets++;
                }
           }
           is_multi_chek = true;
      }
      if(!is_recursion || (is_recursion && is_recursion_with_braket_on_borders)){
          for(int i = 0; i < txt.size(); i++ ){
              x = txt[i];
              if(i == txt.size() - 1) {
                  buffer.push_back(x);
                  str_phars.push_back(buffer);
                  buffer.clear();
                  if(x == ')') {
                      count_brakets++;
                  }
              } else if(x == '+' && !braket_is_cheking) {
                  str_phars.push_back(buffer);
                  buffer.clear();
              } else if(x == ')'){
                  count_brakets++;
                  buffer.push_back(x);
              } else if(x == '(') {
                  braket_is_cheking = true;
                  count_brakets0++;
                  buffer.push_back(x);
              } else if(x == '-' && i == 0 && !braket_is_cheking) {
                  buffer.push_back(x);
              } else if(x == '-' && !braket_is_cheking) {
                  str_phars.push_back(buffer);
                  buffer.clear();
                  buffer.push_back(x);
              } else if(x != '+' && !braket_is_cheking) {
                  buffer.push_back(x);
              }  else {
                  buffer.push_back(x);
              }
              if(x == ')' && count_brakets == count_brakets0) {
                  count_brakets = 0;
                  braket_is_cheking = false;
                  count_main_brakets++;
              }
          }
      }

      for(QString i : str_phars){
          double var = i.toDouble();
          if(i != '0') {
              if(var == 0 && is_multi_chek && count_main_brakets == 1){ // || i.count(')') == 1 )
                   var = simple_count(i, true ,true);
              } else if(var == 0) {
                   var = simple_count(i, true , false);
              }
          }
          int_vars.push_back(var);
      }

      if(is_recursion && !is_recursion_with_braket_on_borders){
          while(!symbols.isEmpty()){
              if(symbols[0] == '*'){
                 int_vars[1] = int_vars[0] * int_vars[1];
                 symbols.pop_front();
                 int_vars.pop_front();
              } else if(symbols[0] == '/'){
                  int_vars[1] = int_vars[0] / int_vars[1];
                  symbols.pop_front();
                  int_vars.pop_front();
              }
          }
          return int_vars[0];
      }

      double result = int_vars[0];
      for(int i = 1 ; i < int_vars.size() ; i++){
            result += int_vars[i];
      }

      return result;
 }
protected slots:
 void b_clk(const QString& str) { if((showLine->text().isEmpty() || showLine->text()[showLine->text().size() - 1] != ')') && !zero_clk) showLine->setText(showLine->text() + str); }
 void bDot_clk(){
     if(!showLine->text().isEmpty() &&  showLine->text()[showLine->text().size() - 1].isNumber()){
          showLine->setText(showLine->text() + ".");
          zero_clk = false;
     }
 }
 void bPlus_clk(){
     if(!showLine->text().isEmpty() &&  showLine->text()[showLine->text().size() - 1].isNumber() || showLine->text()[showLine->text().size() - 1] == ')'){
          showLine->setText(showLine->text() + "+");
          zero_clk = false;
     }
 }
 void bMinus_clk(){
     QString txt = showLine->text();
     if(txt.isEmpty()){
         showLine->setText(showLine->text() + "-");
         zero_clk = false;
     }
     else if((txt[txt.size() - 1].isNumber() || txt[txt.size() - 1] == ')' || txt[txt.size() - 1] == '(')){
          showLine->setText(showLine->text() + "-");
           zero_clk = false;
     }
 }
 void bMulti_clk(){
     QString txt = showLine->text();
     if(!showLine->text().isEmpty() &&  (txt[txt.size() - 1].isNumber() || txt[txt.size() - 1] == ')')){
          showLine->setText(showLine->text() + "*");
          zero_clk = false;
     }
 }
 void bDivide_clk(){
     QString txt = showLine->text();
     if(!showLine->text().isEmpty() &&  (txt[txt.size() - 1].isNumber() || txt[txt.size() - 1] == ')')){
          showLine->setText(showLine->text() + "/");
          zero_clk = false;
     }
 }
 void braket0_clk(){
     if(!showLine->text().isEmpty()){
         if(!showLine->text()[showLine->text().size() - 1].isNumber() || showLine->text()[showLine->text().size() - 1] == '(' || showLine->text()[showLine->text().size() - 1] != ')'){
              showLine->setText(showLine->text() + "(");
              brakets++;
         }
     } else{
         showLine->setText(showLine->text() + "(");
         brakets++;
     }
 }
 void braket_clk(){
     QString txt = showLine->text();
     if(!showLine->text().isEmpty() && brakets > 0 &&  (txt[txt.size() - 1].isNumber() || txt[txt.size() - 1] == ')')){
          showLine->setText(showLine->text() + ")");
          brakets--;
          zero_clk = false;
     }
 }
 void Result_Clicked(){
     if(showLine->text().isEmpty() || (showLine->text()[showLine->text().size() - 1] != ')' && !showLine->text()[showLine->text().size() - 1].isNumber())) { return void(); }
     disconnect(bClear, &QPushButton::clicked, this, &calculator::bClear_clk);

     showLine->setText(QString::number(simple_count(showLine->text(), false, false)));

     connect(bClear, &QPushButton::clicked, this, &calculator::bClear_clk);
 }
 void bClear_clk(){ showLine->setText(""), brakets = 0; }
 void popB_clk(){ // now disable
     showLine->text().remove(showLine->text()[showLine->text().size() - 2]);
 }
public:
 void paintEvent(QPaintEvent *event) override{
     Q_UNUSED(event);
     QPainter p;
     p.begin(this);

     QColor a(200, 220, 255);
     p.setPen(QPen(a, 3, Qt::SolidLine));
     p.setBrush(QBrush(a));
     p.drawRect(QRect(QPoint(0, b1->y() - 10), QSize(width(), height())));
     QColor b(255, 220, 255);
     p.setPen(QPen(b, 3, Qt::SolidLine));
     p.setBrush(QBrush(b));
     p.drawRect(QRect(QPoint(0, 0), QSize(width(), b1->y() - 10)));



     p.end();
 }
 calculator(QWidget *qw = NULL) : QWidget(qw){
     showLine->setEnabled(false);

     showLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     b1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
     b2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     b3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     b4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     b5->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     b6->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     b7->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     b8->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     b9->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     b0->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     bPlus->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     bMinus->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     bMulti->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     bDivide->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     bClear->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
     bDot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     res->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     bBraket0->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     bBraket->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

     bClear->setStyleSheet("background-color: rgb(255, 255, 131);"
                           "padding: 10px;"
                           "border-radius: 10px");
     /*b1->setStyleSheet("QPushButton{"

                       "}"
                       "QPushButton:hover{ "
                       ""
                       "};"
                       "QPushButton:pressed { "
                       "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #dadbde, stop: 1 #f6f7fa); "
                       "}"
                       );*/
     //b1->setStyleSheet();

     //////////////////////////////
     hbl->setSpacing(10);
     hbl->addWidget(showLine);
     hbl->addWidget(bClear);
     //hbl->addWidget(popB);
     //////////////////////////////
     grid->setSpacing(5);

     grid->addWidget(b1, 0, 0);//1
     grid->addWidget(b2, 0, 1);
     grid->addWidget(b3, 0, 2);
     grid->addWidget(bDivide, 0, 3);

     grid->addWidget(b4, 1, 0);//2
     grid->addWidget(b5, 1, 1);
     grid->addWidget(b6, 1, 2);
     grid->addWidget(bMulti, 1, 3);

     grid->addWidget(b7, 2, 0);//3
     grid->addWidget(b8, 2, 1);
     grid->addWidget(b9, 2, 2);
     grid->addWidget(bMinus, 2, 3);

     grid->addWidget(bDot, 3, 0);
     grid->addWidget(b0, 3, 1);
     grid->addWidget(res, 3, 2);
     grid->addWidget(bPlus, 3, 3);
     grid->addWidget(bBraket0, 4, 0);
     grid->addWidget(bBraket, 4, 1);
     //////////////////////////////

     vbl->setSpacing(20);
     vbl->addLayout(hbl);
     vbl->addLayout(grid);
     setLayout(vbl);

     connect(sm, &QSignalMapper::mappedString, this, &calculator::b_clk);
     sm->setMapping(b1, "1");
     sm->setMapping(b2, "2");
     sm->setMapping(b3, "3");
     sm->setMapping(b4, "4");
     sm->setMapping(b5, "5");
     sm->setMapping(b6, "6");
     sm->setMapping(b7, "7");
     sm->setMapping(b8, "8");
     sm->setMapping(b9, "9");
     sm->setMapping(b0, "0");
     connect(b1, SIGNAL(clicked()), sm, SLOT(map()));
     connect(b2, SIGNAL(clicked()), sm, SLOT(map()));
     connect(b3, SIGNAL(clicked()), sm, SLOT(map()));
     connect(b4, SIGNAL(clicked()), sm, SLOT(map()));
     connect(b5, SIGNAL(clicked()), sm, SLOT(map()));
     connect(b6, SIGNAL(clicked()), sm, SLOT(map()));
     connect(b7, SIGNAL(clicked()), sm, SLOT(map()));
     connect(b8, SIGNAL(clicked()), sm, SLOT(map()));
     connect(b9, SIGNAL(clicked()), sm, SLOT(map()));
     connect(b0, SIGNAL(clicked()), sm, SLOT(map()));
     connect(bDot, &QPushButton::clicked, this, &calculator::bDot_clk);
     connect(bClear, &QPushButton::clicked, this, &calculator::bClear_clk);
     connect(bPlus, &QPushButton::clicked, this, &calculator::bPlus_clk);
     connect(bMinus, &QPushButton::clicked, this, &calculator::bMinus_clk);
     connect(bMulti, &QPushButton::clicked, this, &calculator::bMulti_clk);
     connect(bDivide, &QPushButton::clicked, this, &calculator::bDivide_clk);
     connect(bBraket0, &QPushButton::clicked, this, &calculator::braket0_clk);
     connect(bBraket, &QPushButton::clicked, this, &calculator::braket_clk);
     connect(res, &QPushButton::clicked, this, &calculator::Result_Clicked);
     //connect(popB, &QPushButton::clicked, this, &calculator::popB_clk);
 }
 void resizeEvent(QResizeEvent *event) override {
     QFont font = showLine->font();
     font.setPixelSize(height() * 0.1);
     showLine->setFont(font);
     b0->setFont(font);
     b1->setFont(font);
     b2->setFont(font);
     b3->setFont(font);
     b4->setFont(font);
     b5->setFont(font);
     b6->setFont(font);
     b7->setFont(font);
     b8->setFont(font);
     b9->setFont(font);
     bClear->setFont(font);
     bMulti->setFont(font);
     bMinus->setFont(font);
     bPlus->setFont(font);
     bDot->setFont(font);
     bDivide->setFont(font);
     res->setFont(font);
     bBraket->setFont(font);
     bBraket0->setFont(font);
 }
};

int main(int argc, char *argv[])
{
 QApplication a(argc, argv);

 calculator *cal = new calculator;

 cal->show();
 return a.exec();
}



