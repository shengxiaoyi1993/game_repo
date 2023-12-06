#ifndef chess_h
#define chess_h

/// player
/// board
/// PlayerSelectRule
/// WinnerRule

#include <iostream>
#include <vector>

enum State{
  none=0,
  white,
  black
};



class Board
{
public:
  Board(unsigned long v_width,unsigned long v_height);
  int setNode(unsigned long v_x,unsigned long v_y,State v_state);
  const std::vector<std::vector<State>> & getBoard() const ;
  State getNode(unsigned long v_x,unsigned long v_y) const ;

private:
  unsigned long __width;
  unsigned long __height;
  std::vector<std::vector<State>> __board;
};


class ChessStrategy{
public:
  virtual int planNext(const Board &v_board,unsigned long &v_x,unsigned long &v_y) = 0;
  virtual ~ChessStrategy()=default;
};

class Player
{
public:
  Player();
  void setChessType(State v_state);
  void setChessStrategy(ChessStrategy* pstrategy);
  State getState() const;

  /// 0 落子
  /// 1 认输
  /// 2 悔棋
  int planNext(const Board & v_voard,unsigned long & v_x,unsigned long & v_y);

private:
  State __state;
  ChessStrategy* __strategy;

};


class WinCondition{
public:
  virtual State operator()(const Board & v_board) =0 ;
  virtual ~WinCondition() =default ;
};

class Sequencejudgment
{
public:
  virtual unsigned long getNextPlayer() =0 ;
  virtual ~Sequencejudgment() = default ;
  virtual unsigned long getMaxSeq() =0 ;
};

class ChessTypejudgment
{
public:
  virtual State getPlayerChessType(unsigned long) =0 ;
  virtual ~ChessTypejudgment() = default ;
//  virtual unsigned long getMaxSeq() =0 ;
};



class Game
{
public:
  Game(unsigned long v_width,unsigned long v_height,unsigned long nplayers);
  ~Game();

  void setPlayerChessStrategy(ChessStrategy* pchess_strategy,unsigned long i_player);
  void setPlayerChessType(ChessTypejudgment* pchesstype );
  void setPlayerSeq(Sequencejudgment* pseq);
  void setWinCondition(WinCondition *pwin);
  State start();

private:
  void _update();


private:
  Board __board;
  std::vector<Player> __list_player;
  WinCondition* __judge_win;
  Sequencejudgment* __judge_seq;
  ChessTypejudgment* __judge_chess_type;

};



#endif
