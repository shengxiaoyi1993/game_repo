#include <iostream>
#include <chess/chess.h>

/// 输出0|1
/// 首次输出0,然后依次交替输出1,0,1...
class SimpleSequencejudgment:public Sequencejudgment
{
public:
  SimpleSequencejudgment():__count(0)
  {

  }
  unsigned long getNextPlayer()
  {
     unsigned long result= __count%2;
     __count++;
     return result;
  }
  ~SimpleSequencejudgment()
  {

  }
  unsigned long getMaxSeq()
  {
    return 1;
  }
private:
  unsigned long __count;

};

class SimpleChessStrategy:public ChessStrategy
{
public:
  SimpleChessStrategy()=default;
  int planNext(const Board &v_board,unsigned long &v_x,unsigned long &v_y)
  {
    auto array=v_board.getBoard();
    unsigned long h=array.size();
    if (h == 0 ) {
      throw "array.size == 0";
    }
    unsigned long w=array[0].size();
    for (unsigned long ih=0;ih<h;ih++)
    {
      for (unsigned long iw=0;iw<w;iw++) {
        if (array[ih][iw] == none) {
          v_x=iw ;
          v_y=ih;
          return 0;
        }
      }
    }
    /// 无子可下,让系统判断
    return 2;
  }
  ~SimpleChessStrategy()=default;
};

class SimpleChessTypejudgment:public ChessTypejudgment
{
public:
  State getPlayerChessType(unsigned long v_index)
  {
    if (v_index == 0) {
      return white;
    }
    else if (v_index == 1) {
      return black;
    }
    else{
      return none;
    }

  }
  ~SimpleChessTypejudgment(){}
};

/// 若black,white中任意一种,横向/纵向/对角线方向连续均为同一状态则认为该棋胜利
class FiveWinCondition:public WinCondition
{


public:
  State operator()(const Board & v_board)
  {
    auto array=v_board.getBoard();
    unsigned long h=array.size();
    if (h == 0 ) {
      throw "h == 0";
    }
    unsigned long w=array[0].size();
    if (w == 0 ) {
      throw "w == 0";
    }

    for(unsigned long ih=0;ih<h;ih++)
    {
      for(unsigned long iw=0;iw<w;iw++)
      {
        if (array[ih][iw] != none)
        {
          /// 横向
          if (iw+4 < w
              && array[ih][iw] == array[ih][iw+1]
              && array[ih][iw] == array[ih][iw+2]
              && array[ih][iw] == array[ih][iw+3]
              && array[ih][iw] == array[ih][iw+4])
          {
            return array[ih][iw];
          }

          /// 纵向
          if (ih+4 < h
              && array[ih][iw] == array[ih+1][iw]
              && array[ih][iw] == array[ih+2][iw]
              && array[ih][iw] == array[ih+3][iw]
              && array[ih][iw] == array[ih+4][iw])
          {
            return array[ih][iw];
          }

          /// 对角线
          if (ih+4 < h && iw+4 < w
              && array[ih][iw] == array[ih+1][iw+1]
              && array[ih][iw] == array[ih+2][iw+2]
              && array[ih][iw] == array[ih+3][iw+3]
              && array[ih][iw] == array[ih+4][iw+4])
          {
            return array[ih][iw];
          }
          /// 反对角线
          if (ih > 4 && iw > 4
              && array[ih][iw] == array[ih-1][iw-1]
              && array[ih][iw] == array[ih-2][iw-2]
              && array[ih][iw] == array[ih-3][iw-3]
              && array[ih][iw] == array[ih-4][iw-4])
          {
            return array[ih][iw];
          }

        }
      }
    }
    return none;

  }
  ~FiveWinCondition()=default;

};



int main(int argc, char *argv[])
{
  try {
    Game game_chess(50,50,2);
    Sequencejudgment* pseq =new SimpleSequencejudgment();
    ChessTypejudgment* pchesstype=new SimpleChessTypejudgment();
    WinCondition *pwin=new FiveWinCondition();
    ChessStrategy* pchess_strategy =new SimpleChessStrategy();
    game_chess.setPlayerChessType(pchesstype);
    game_chess.setPlayerChessStrategy(pchess_strategy,0);
    game_chess.setPlayerChessStrategy(pchess_strategy,1);
    game_chess.setPlayerSeq(pseq);
    game_chess.setWinCondition(pwin);

    State win_state= game_chess.start();
    std::cout<<"win_state:"<<win_state<<std::endl;
  } catch (char const* ex) {
    std::cout<<"ex:"<<ex<<std::endl;
  }


  return 0;
}

