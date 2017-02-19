/*
Rodent, a UCI chess playing engine derived from Sungorus 1.4
Copyright (C) 2009-2011 Pablo Vazquez (Sungorus author)
Copyright (C) 2011-2017 Pawel Koziol

Rodent is free software: you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

Rodent is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/

#include "rodent.h"

int *GenerateCaptures(POS *p, int *list) {

  U64 pieces, moves;
  int sd, from, to;

  sd = p->side;
  if (sd == WC) {
    moves = ((p->Pawns(WC) & ~FILE_A_BB & RANK_7_BB) << 7) & p->cl_bb[BC];
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (Q_PROM << 12) | (to << 6) | (to - 7);
      *list++ = (R_PROM << 12) | (to << 6) | (to - 7);
      *list++ = (B_PROM << 12) | (to << 6) | (to - 7);
      *list++ = (N_PROM << 12) | (to << 6) | (to - 7);
    }

    moves = ((p->Pawns(WC) & ~FILE_H_BB & RANK_7_BB) << 9) & p->cl_bb[BC];
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (Q_PROM << 12) | (to << 6) | (to - 9);
      *list++ = (R_PROM << 12) | (to << 6) | (to - 9);
      *list++ = (B_PROM << 12) | (to << 6) | (to - 9);
      *list++ = (N_PROM << 12) | (to << 6) | (to - 9);
    }

    moves = ((p->Pawns(WC) & RANK_7_BB) << 8) & UnoccBb(p);
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (Q_PROM << 12) | (to << 6) | (to - 8);
      *list++ = (R_PROM << 12) | (to << 6) | (to - 8);
      *list++ = (B_PROM << 12) | (to << 6) | (to - 8);
      *list++ = (N_PROM << 12) | (to << 6) | (to - 8);
    }

    moves = ((p->Pawns(WC) & ~FILE_A_BB & ~RANK_7_BB) << 7) & p->cl_bb[BC];
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | (to - 7);
    }

    moves = ((p->Pawns(WC) & ~FILE_H_BB & ~RANK_7_BB) << 9) & p->cl_bb[BC];
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | (to - 9);
    }

    if ((to = p->ep_sq) != NO_SQ) {
      if (((p->Pawns(WC) & ~FILE_A_BB) << 7) & SqBb(to))
        *list++ = (EP_CAP << 12) | (to << 6) | (to - 7);
      if (((p->Pawns(WC) & ~FILE_H_BB) << 9) & SqBb(to))
        *list++ = (EP_CAP << 12) | (to << 6) | (to - 9);
    }
  } else {
    moves = ((p->Pawns(BC) & ~FILE_A_BB & RANK_2_BB) >> 9) & p->cl_bb[WC];
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (Q_PROM << 12) | (to << 6) | (to + 9);
      *list++ = (R_PROM << 12) | (to << 6) | (to + 9);
      *list++ = (B_PROM << 12) | (to << 6) | (to + 9);
      *list++ = (N_PROM << 12) | (to << 6) | (to + 9);
    }

    moves = ((p->Pawns(BC) & ~FILE_H_BB & RANK_2_BB) >> 7) & p->cl_bb[WC];
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (Q_PROM << 12) | (to << 6) | (to + 7);
      *list++ = (R_PROM << 12) | (to << 6) | (to + 7);
      *list++ = (B_PROM << 12) | (to << 6) | (to + 7);
      *list++ = (N_PROM << 12) | (to << 6) | (to + 7);
    }

    moves = ((p->Pawns(BC) & RANK_2_BB) >> 8) & UnoccBb(p);
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (Q_PROM << 12) | (to << 6) | (to + 8);
      *list++ = (R_PROM << 12) | (to << 6) | (to + 8);
      *list++ = (B_PROM << 12) | (to << 6) | (to + 8);
      *list++ = (N_PROM << 12) | (to << 6) | (to + 8);
    }

    moves = ((p->Pawns(BC) & ~FILE_A_BB & ~RANK_2_BB) >> 9) & p->cl_bb[WC];
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | (to + 9);
    }

    moves = ((p->Pawns(BC) & ~FILE_H_BB & ~RANK_2_BB) >> 7) & p->cl_bb[WC];
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | (to + 7);
    }

    if ((to = p->ep_sq) != NO_SQ) {
      if (((p->Pawns(BC) & ~FILE_A_BB) >> 9) & SqBb(to))
        *list++ = (EP_CAP << 12) | (to << 6) | (to + 9);
      if (((p->Pawns(BC) & ~FILE_H_BB) >> 7) & SqBb(to))
        *list++ = (EP_CAP << 12) | (to << 6) | (to + 7);
    }
  }

  pieces = p->Knights(sd);
  while (pieces) {
    from = BB.PopFirstBit(&pieces);
    moves = BB.KnightAttacks(from) & p->cl_bb[Opp(sd)];
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | from;
    }
  }

  pieces = p->Bishops(sd);
  while (pieces) {
    from = BB.PopFirstBit(&pieces);
    moves = BB.BishAttacks(OccBb(p), from) & p->cl_bb[Opp(sd)];
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | from;
    }
  }

  pieces = p->Rooks(sd);
  while (pieces) {
    from = BB.PopFirstBit(&pieces);
    moves = BB.RookAttacks(OccBb(p), from) & p->cl_bb[Opp(sd)];
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | from;
    }
  }

  pieces = p->Queens(sd);
  while (pieces) {
    from = BB.PopFirstBit(&pieces);
    moves = BB.QueenAttacks(OccBb(p), from) & p->cl_bb[Opp(sd)];
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | from;
    }
  }

  moves = BB.KingAttacks(KingSq(p, sd)) & p->cl_bb[Opp(sd)];
  while (moves) {
    to = BB.PopFirstBit(&moves);
    *list++ = (to << 6) | KingSq(p, sd);
  }
  return list;
}

int *GenerateQuiet(POS *p, int *list) {

  U64 pieces, moves;
  int sd, from, to;

  sd = p->side;
  if (sd == WC) {
    if ((p->c_flags & W_KS) && !(OccBb(p) & (U64)0x0000000000000060))
      if (!Attacked(p, E1, BC) && !Attacked(p, F1, BC))
        *list++ = (CASTLE << 12) | (G1 << 6) | E1;
    if ((p->c_flags & W_QS) && !(OccBb(p) & (U64)0x000000000000000E))
      if (!Attacked(p, E1, BC) && !Attacked(p, D1, BC))
        *list++ = (CASTLE << 12) | (C1 << 6) | E1;
    moves = ((((p->Pawns(WC) & RANK_2_BB) << 8) & UnoccBb(p)) << 8) & UnoccBb(p);

    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (EP_SET << 12) | (to << 6) | (to - 16);
    }

    moves = ((p->Pawns(WC) & ~RANK_7_BB) << 8) & UnoccBb(p);
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | (to - 8);
    }
  } else {
    if ((p->c_flags & B_KS) && !(OccBb(p) & (U64)0x6000000000000000))
      if (!Attacked(p, E8, WC) && !Attacked(p, F8, WC))
        *list++ = (CASTLE << 12) | (G8 << 6) | E8;
    if ((p->c_flags & B_QS) && !(OccBb(p) & (U64)0x0E00000000000000))
      if (!Attacked(p, E8, WC) && !Attacked(p, D8, WC))
        *list++ = (CASTLE << 12) | (C8 << 6) | E8;
    moves = ((((p->Pawns(BC) & RANK_7_BB) >> 8) & UnoccBb(p)) >> 8) & UnoccBb(p);

    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (EP_SET << 12) | (to << 6) | (to + 16);
    }

    moves = ((p->Pawns(BC) & ~RANK_2_BB) >> 8) & UnoccBb(p);
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | (to + 8);
    }
  }

  pieces = p->Knights(sd);
  while (pieces) {
    from = BB.PopFirstBit(&pieces);
    moves = BB.KnightAttacks(from) & UnoccBb(p);
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | from;
    }
  }

  pieces = p->Bishops(sd);
  while (pieces) {
    from = BB.PopFirstBit(&pieces);
    moves = BB.BishAttacks(OccBb(p), from) & UnoccBb(p);
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | from;
    }
  }

  pieces = p->Rooks(sd);
  while (pieces) {
    from = BB.PopFirstBit(&pieces);
    moves = BB.RookAttacks(OccBb(p), from) & UnoccBb(p);
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | from;
    }
  }

  pieces = p->Queens(sd);
  while (pieces) {
    from = BB.PopFirstBit(&pieces);
    moves = BB.QueenAttacks(OccBb(p), from) & UnoccBb(p);
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | from;
    }
  }

  moves = BB.KingAttacks(KingSq(p, sd)) & UnoccBb(p);
  while (moves) {
    to = BB.PopFirstBit(&moves);
    *list++ = (to << 6) | KingSq(p, sd);
  }
  return list;
}

int *GenerateSpecial(POS *p, int *list) {

  U64 pieces, moves;
  int sd, from, to;
  int ksq = KingSq(p, Opp(p->side));
  U64 n_check = BB.KnightAttacks(ksq);
  U64 r_check = BB.RookAttacks(OccBb(p), ksq);
  U64 b_check = BB.BishAttacks(OccBb(p), ksq);
  U64 q_check = r_check | b_check;
  U64 p_check = BB.ShiftFwd(BB.ShiftSideways(SqBb(ksq)), Opp(p->side));

  sd = p->side;
  int op = Opp(sd);

  if (sd == WC) {

    moves = ((((p->Pawns(WC) & RANK_2_BB) << 8) & UnoccBb(p)) << 8) & UnoccBb(p);
	moves = moves & p_check;
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (EP_SET << 12) | (to << 6) | (to - 16);
    }

    moves = ((p->Pawns(WC) & ~RANK_7_BB) << 8) & UnoccBb(p);
	moves = moves & p_check;
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | (to - 8);
    }
  } else {
    moves = ((((p->Pawns(BC) & RANK_7_BB) >> 8) & UnoccBb(p)) >> 8) & UnoccBb(p);
	moves = moves & p_check;
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (EP_SET << 12) | (to << 6) | (to + 16);
    }

    moves = ((p->Pawns(BC) & ~RANK_2_BB) >> 8) & UnoccBb(p);
	moves = moves & p_check;
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | (to + 8);
    }
  }

  pieces = p->Knights(sd);
  while (pieces) {
    from = BB.PopFirstBit(&pieces);
	int knight_discovers = 0;
	
    U64 bb_checkers = p->Queens(sd) | p->Rooks(sd) | p->Bishops(sd);
    while (bb_checkers) {
      int checker = BB.PopFirstBit(&bb_checkers);
      U64 bb_ray = BB.bbBetween[checker][p->king_sq[op]];

      if (SqBb(from) & bb_ray) {
        if (BB.PopCnt(bb_ray & OccBb(p)) == 1) {
           knight_discovers = 1;
           break;
        }
      }
    }
   
	moves = BB.KnightAttacks(from) & UnoccBb(p);
	if (!knight_discovers) moves = moves & n_check;
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | from;
    }
  }

  pieces = p->Bishops(sd);
  while (pieces) {
    from = BB.PopFirstBit(&pieces);
	int bish_discovers = 0;
	
	U64 bb_checkers = p->StraightMovers(sd);
    while (bb_checkers) {
      int checker = BB.PopFirstBit(&bb_checkers);
      U64 bb_ray = BB.bbBetween[checker][p->king_sq[op]];

      if (SqBb(from) & bb_ray) {
        if (BB.PopCnt(bb_ray & OccBb(p)) == 1) {
           bish_discovers = 1;
           break;
        }
      }
    }
    
    moves = BB.BishAttacks(OccBb(p), from) & UnoccBb(p);
	if (!bish_discovers) moves = moves & b_check;
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | from;
    }
  }

  pieces = p->Rooks(sd);
  while (pieces) {
    from = BB.PopFirstBit(&pieces);
	int rook_discovers = 0;
	
	U64 bb_checkers = p->DiagMovers(sd);
    while (bb_checkers) {
      int checker = BB.PopFirstBit(&bb_checkers);
      U64 bb_ray = BB.bbBetween[checker][p->king_sq[op]];

      if (SqBb(from) & bb_ray) {
        if (BB.PopCnt(bb_ray & OccBb(p)) == 1) {
           rook_discovers = 1;
           break;
        }
      }
    }
    
    moves = BB.RookAttacks(OccBb(p), from) & UnoccBb(p);
	if (!rook_discovers) moves = moves & r_check;
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | from;
    }
  }

  pieces = p->Queens(sd);
  while (pieces) {
    from = BB.PopFirstBit(&pieces);
    moves = BB.QueenAttacks(OccBb(p), from) & UnoccBb(p);
	moves = moves & q_check;
    while (moves) {
      to = BB.PopFirstBit(&moves);
      *list++ = (to << 6) | from;
    }
  }

  /*
  moves = k_attacks[KingSq(p, sd)] & UnoccBb(p);
  while (moves) {
    to = BB.PopFirstBit(&moves);
    *list++ = (to << 6) | KingSq(p, sd);
  }
  */
  return list;
}