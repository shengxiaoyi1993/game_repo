#include "chess/chess.h"


Board::Board(unsigned long v_width,unsigned long v_height):
  __width(v_width),
  __height(v_height),
  __board(__height,std::vector<State>(__width))
{
  if (__width==0 || __height==0) {
    throw "__width==0 || __height==0";
  }
}

int
Board::setNode(unsigned long v_x,unsigned long v_y,State v_state)
{
  if (v_x>=__width|| v_y>=__height) {
    throw "v_x>=__width|| v_y>=__height";
  }
//  std::cout<<"("<<v_x<<","<<v_y<<") "<<v_state<<std::endl;
  __board[v_y][v_x]=v_state;
  return 0;
}

const std::vector<std::vector<State>> &
Board::getBoard()const
{
  return __board;
}


State
Board::getNode(unsigned long v_x,unsigned long v_y)const
{
  return __board[v_y][v_x];
}
//////////////////Player/////////////////////
Player::Player():__state(none),__strategy(nullptr)
{

}

void Player::setChessType(State v_state)
{
  __state=v_state;
}


int Player::planNext(const Board & v_voard,unsigned long & v_x,unsigned long & v_y)
{
  if (__strategy == nullptr) {
    throw "__strategy == nullptr";
  }
  return __strategy->planNext(v_voard,v_x,v_y);
}

void
Player::setChessStrategy(ChessStrategy* pstrategy)
{
  __strategy=pstrategy;
}


State Player::getState() const
{
  return __state;
}

///////////////////Game//////////////////////
Game::Game(unsigned long v_width,
           unsigned long v_height,
           unsigned long nplayers)
  :__board(v_width,v_height),
    __list_player(nplayers),
    __judge_win(nullptr),
    __judge_seq(nullptr)
{}

Game::~Game(){}


void
Game::setPlayerChessStrategy(ChessStrategy* pchess_strategy,unsigned long i_player)
{
  if (i_player>= __list_player.size()) {
    throw "i_player>= __list_player.size()";
  }
  __list_player[i_player].setChessStrategy(pchess_strategy);
}
void
Game::setPlayerChessType(ChessTypejudgment* pchesstype )
{
  __judge_chess_type=pchesstype;
}

void
Game::setPlayerSeq(Sequencejudgment* pseq)
{
  __judge_seq=pseq;
}

void
Game::setWinCondition(WinCondition *pwin)
{
  __judge_win=pwin;
}

void Game::_update()
{
  for(const auto &list:__board.getBoard()){
    std::cout<<"|";
    for(const auto &it:list)
    {
      if (it==white) {
        std::cout<<"O|";
      }
      else if(it== black){
        std::cout<<"X|";
      }
      else{
        std::cout<<" |";
      }
    }
    std::cout<<std::endl;
  }
}


State Game::start()
{
  if (__judge_seq== nullptr ) {
    throw "__judge_seq== nullptr";
  }
  if (__judge_win== nullptr ) {
    throw "__judge_win== nullptr";
  }
  if (__list_player.size() == 0) {
    throw "__list_player.size() == 0";
  }

  if (__judge_seq->getMaxSeq() >= __list_player.size() ) {
    throw "__judge_seq->getMaxSeq() >= __list_player.size()";
  }

  for (unsigned long i=0;i<__list_player.size();i++) {
    __list_player[i].setChessType(__judge_chess_type->getPlayerChessType(i));
  }


  while ( (*__judge_win)(__board) == none ) {
    unsigned long step_player = __judge_seq->getNextPlayer();
//    std::cout<<"step_player:"<<step_player<<std::endl;
    unsigned long posx,posy;
    int result =__list_player[step_player].planNext(__board,posx,posy);
    if (result == 0) {
      __board.setNode(posx,posy,__list_player[step_player].getState());
      _update();
    }
    else if(result == 1)
    {
      return __list_player[step_player].getState()==white?black:white;
    }
    else{
      std::cout<<"not support"<<std::endl;
      return none;
    }
  }

  return (*__judge_win)(__board);

}

