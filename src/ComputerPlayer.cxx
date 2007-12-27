/*
 * ComputerPlayer.cxx
 * Andrew Sayman - 3/27/05
 *
 * Copyright (C) 2005  Andrew Sayman
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "ComputerPlayer.h"

#include <cassert>
#include "LevelLights.h"
#include "Score.h"
#include "Controller.h"
#include "BlockManager.h"
#include "Grid.h"
#include "Swapper.h"

#include <sstream>

using namespace std;

//#define WAIT_TIME ( GC_STEPS_PER_SECOND * 10 )

bool ComputerPlayer::lost;
bool ComputerPlayer::_impact;
ComputerPlayerAI *ComputerPlayer::ai;
int ComputerPlayer::start_time = 0;
int ComputerPlayer::alarm = 0;
std::vector< PathPortion > ComputerPlayer::path;
std::vector< ComboAccounting > ComputerPlayer::last_choices;

ostream& operator<< (ostream& os, const Path& path)
{
  if (path.empty())
    return os;
  os << "(" << path[0].current_x << "," << path[0].current_y << ")-";
  for (size_t i = 0; i < path.size(); ++i) {
    if (path[i].key_action == GC_SWAP_KEY) {
      os << "SWAP-";
    } else {
      os << "(" << path[i].after_x << "," << path[i].after_y << ")-";
    }
  }
  return os;
}

static bool has_row_path_between(int x1, int x2, int row)
{
  int lesser_x, greater_x;
  if (x1 > x2) {
    lesser_x = x2;
    greater_x = x1;
  } else {
    lesser_x = x1;
    greater_x = x2;
  }

  if (x1 == x2)
    return true;

  // The simple case is that all of the things between the two locations are blocks
  // This means that there can be no falling holes

  bool all_blocks = true;
  for (int counter_x = lesser_x; counter_x <= greater_x; ++counter_x) {
    if (GR_BLOCK != Grid::stateAt(counter_x, row)) {
      all_blocks = false;
    }
  }
  if (all_blocks) {
    //MESSAGE("Returning that they're all blocks");
    return true;
  }

  // The complicated case is an empty, because we need to check that the state below
  // is one in which we won't fall

  bool will_fall = false;
  for (int counter_x = lesser_x; counter_x <= greater_x; ++counter_x) {
    if (GR_EMPTY == Grid::stateAt(counter_x, row)) {
      if (row > 1) {
        if (GR_EMPTY == Grid::stateAt(counter_x, row - 1)) {
          will_fall = true;
          break;
        }
      }
    }
  }
  if (will_fall) {
    //MESSAGE("Will fall");
    return false;
  } else {
    //MESSAGE("Won't fall");
  }

  // Also have to make sure there aren't any of the special things blocking us.
  // At this point we know that they are not all blocks, but any empty spaces have
  // no holes. Any areas that aren't blocks or empty then must be something else that
  // can block us
  for (int counter_x = lesser_x; counter_x <= greater_x; ++counter_x) {
    if (GR_EMPTY != Grid::stateAt(counter_x, row)) {
      if (GR_BLOCK != Grid::stateAt(counter_x, row)) {
        //MESSAGE("Found non-empty and non-block");
        return false;
      }
    }
  }

  //MESSAGE("Block can move in its row!");
  return true;
}

static std::vector<int> row_flavors(int row, int flavor, int closest_to = 0)
{
  std::vector<int> locations;

  assert(closest_to < GC_PLAY_WIDTH);
  assert(closest_to >= 0);

  if (0 == closest_to) {
    for (int x = 0; x < GC_PLAY_WIDTH; ++x) {
      if (GR_BLOCK == Grid::stateAt(x, row)) {
        if (Grid::flavorAt(x, row) == flavor) {
          locations.push_back(x);
        }
      }
    }
    return locations;
  }

  int sign = 1;
  int idx = closest_to;
  int count = 0;
  const int max_count = 12;

  while (count <= max_count) {
    idx += (count * sign);
    if ((idx >= 0) && (idx < GC_PLAY_WIDTH)) {
      if ((GR_BLOCK == Grid::stateAt(idx, row)) &&
          (Grid::flavorAt(idx, row) == flavor)) {
        locations.push_back(idx);
      }
    }
    ++count;
    sign *= -1;
  }

  return locations;
}

static std::vector< PathPortion > path_between(int start_x, int start_y, int end_x, int end_y, int move_delay = GC_MOVE_DELAY)
{
  std::vector< PathPortion > ret_path;
  int x_move = 0, y_move = 0;
  int dir, inc;
  int bound_end_x = end_x;
  int current_x, current_y;

  if (bound_end_x > GC_PLAY_WIDTH - 2) {
    bound_end_x = GC_PLAY_WIDTH - 2;
  }

  x_move = start_x - bound_end_x;
  y_move = start_y - end_y;

  current_x = start_x; current_y = start_y;
  if (!(0 == x_move)) {
    if (x_move < 0) {
      dir = GC_RIGHT_KEY;
      inc = 1;
    } else {
      dir = GC_LEFT_KEY;
      inc = -1;
    }
    for (; x_move != 0; x_move += inc) {
      PathPortion p;
      p.alarm = move_delay;
      p.key_action = dir;
      p.target_x = bound_end_x;
      p.target_y = end_y;

      p.current_x = current_x;
      p.current_y = current_y;
      current_x += inc;
      p.after_x = current_x;
      p.after_y = current_y;

      ret_path.push_back(p);
    }
  }
  if (!(0 == y_move)) {
    if (y_move < 0) {
      dir = GC_UP_KEY;
      inc = 1;
    } else {
      dir = GC_DOWN_KEY;
      inc = -1;
    }
    for (; y_move != 0; y_move += inc) {
      PathPortion p;
      p.alarm = move_delay;
      p.key_action = dir;
      p.target_x = bound_end_x;
      p.target_y = end_y;

      p.current_x = current_x;
      p.current_y = current_y;
      current_y += inc;
      p.after_x = current_x;
      p.after_y = current_y;

      ret_path.push_back(p);
    }
  }


  return ret_path;
}

static Path swap_between(int swap_x, int swap_y, int start_x, int end_x, int row, int swap_delay = GC_SWAP_DELAY, int move_delay = GC_MOVE_DELAY)
{
  Path ret_path;
  int x_move = 0, current_x, current_y;
  int dir, inc;

  x_move = start_x - end_x;

  if (0 == x_move) {
    return ret_path;
  } else {
    if (start_x > end_x) {
      current_x = start_x - 1;
      current_y = row;
      std::vector< PathPortion > additional_path = path_between(
          swap_x, swap_y,
          current_x, current_y,
          move_delay);
      if (!additional_path.empty())
        ret_path.insert(ret_path.end(), additional_path.begin(), additional_path.end());
    } else {
      current_x = start_x;
      current_y = row;
      std::vector< PathPortion > additional_path = path_between(
          swap_x, swap_y,
          current_x, current_y,
          move_delay);
      if (!additional_path.empty())
        ret_path.insert(ret_path.end(), additional_path.begin(), additional_path.end());
    }
  }
  /*
  for (size_t i = 0; i < ret_path.size(); ++i) {
    ret_path[i].destination = std::make_pair(end_x, row);
  }
  */

  if (x_move < 0) {
    dir = GC_RIGHT_KEY;
    inc = 1;
  } else {
    dir = GC_LEFT_KEY;
    inc = -1;
  }
  while (x_move != 0) {
    PathPortion p;
    p.alarm = swap_delay;
    p.key_action = GC_SWAP_KEY;
    p.target_x = start_x;
    p.target_y = row;
    //p.destination = std::make_pair(end_x, row);
    p.current_x = p.after_x = current_x;
    p.current_y = p.after_y = current_y;

    ret_path.push_back(p);

    x_move += inc;
    if (x_move != 0) {
      PathPortion p;
      p.alarm = move_delay;
      p.key_action = dir;
      p.target_x = start_x;
      p.target_y = row;
      //p.destination = std::make_pair(end_x, row);

      p.current_x = current_x;
      p.current_y = current_y;
      current_x += inc;
      p.after_x = current_x;
      p.after_y = current_y;

      ret_path.push_back(p);
    }
  }

  return ret_path;
}

static std::vector< PathPortion > gravity_flavor_path(int swap_x, int swap_y, int current_flavor, int x, int y, int &chosen_x)
{
  std::vector< PathPortion > ret_path;
  std::vector<int> locations = row_flavors(y, current_flavor, x);
  if (locations.empty()) {
    return ret_path;
  }
  std::vector<bool> has_path;
  for (unsigned int i = 0; i < locations.size(); ++i) {
    has_path.push_back(
        has_row_path_between(
          x, locations[i], y));
  }
  for (unsigned int i = 0; i < has_path.size(); ++i) {
    if (has_path[i]) {
      std::vector< PathPortion > additional_path = swap_between(
          swap_x, swap_y,
          locations[i], x, y);
          //GC_SWAP_DELAY * 3, 50);
      if (!additional_path.empty()) {
        ret_path.insert(ret_path.end(), additional_path.begin(), additional_path.end());
        chosen_x = locations[i];
        return ret_path;
      }
    }
  }
  return ret_path;
}

static std::vector<int> row_threshold_flavors(int row, size_t threshold = 3)
{
  assert(threshold > 0);
  size_t flavor_counts[BF_NUMBER];
  std::vector<int> flavors_matching_threshold;
  ostringstream s;

  for (size_t i = 0; i < BF_NUMBER; ++i) {
    flavor_counts[i] = 0;
  }

  for (size_t i = 0; i < GC_PLAY_WIDTH; ++i) {
    if (GR_BLOCK == Grid::stateAt(i, row)) {
      ++flavor_counts[Grid::flavorAt(i, row)];
    }
  }

  for (size_t i = 0; i < BF_NUMBER; ++i) {
    if (flavor_counts[i] >= threshold) {
      flavors_matching_threshold.push_back(i);
    }
  }

  if (!flavors_matching_threshold.empty()) {
    s << row << " should have horz combo" << ends;
    MESSAGE("Row " << s.str());
  }

  return flavors_matching_threshold;
}

static Path choose_shortest_path(Paths paths)
{
  // Use numeric limits instead
  const int path_max = 2000000;
  const int path_null = -1;
  std::pair<int, int> least_path(path_max, path_null);

  if (!paths.empty()) {
    std::ostringstream s, p, r;
    s << paths.size() << std::ends;
    MESSAGE("# of paths to choose from " << s.str());
    ComputerPlayer::last_choices.clear();
    for (size_t i = 0; i < paths.size(); ++i) {
      ComputerPlayer::last_choices.push_back(paths[i][0].accounting);
      p << " [" << i << ": " << paths[i].size() << "]";
    }
    MESSAGE("Sizes " << p.str());
    for (size_t i = 0; i < paths.size(); ++i) {
      if (paths[i].size() < least_path.first) {
        least_path = std::make_pair(paths[i].size(), i);
      }
    }
    if (least_path.second != path_null) {
      r << least_path.second;
      MESSAGE("Picking " << r.str());
      return paths[least_path.second];
    }
  } else {
    MESSAGE("Choosing from empty paths");
  }

  Path blank_path;
  return blank_path;
}

static Path generate_horizontal_swap_path(int swap_x, int swap_y, size_t first, size_t second, size_t third, size_t y)
{
  Path ret_path, one, two;
  int current_flavor = Grid::flavorAt(first, y);
  bool has_path[2] = {false, false}, has_match[2] = {false, false};
  size_t target_for_second, target_for_third;
  ostringstream s, p;
  s << "first " << first << " second " << second << " third " << third << ends;
  MESSAGE("HorzSwapPath " << s.str());

  assert(second < third);

  if (second < first) {
    target_for_second = first - 1;
  } else {
    target_for_second = first + 1;
  }

  if (third < first) {
    if (second < first) {
      target_for_third = first - 2;
    } else {
      target_for_third = first - 1;
    }
  } else {
    if (second < first) {
      target_for_third = first + 1;
    } else {
      target_for_third = first + 2;
    }
  }

  p << "second " << target_for_second << " third " << target_for_third << ends;
  MESSAGE("HorzSwapTargets " << p.str());

  if ((target_for_second < 0) || (target_for_second >= GC_PLAY_WIDTH) ||
      (target_for_third < 0) || (target_for_third >= GC_PLAY_WIDTH)) {
    MESSAGE("Potentials out of bounds");
    return ret_path;
  }

  if ((!has_row_path_between(target_for_second, second, y)) ||
      (!has_row_path_between(target_for_third, third, y))) {
    MESSAGE("No row path");
    return ret_path;
  }

  if ((GR_BLOCK == Grid::stateAt(target_for_second, y)) &&
      (Grid::flavorAt(target_for_second, y) == current_flavor)) {
    has_match[0] = true;
  } else {
    one = swap_between(
        swap_x, swap_y,
        second, target_for_second, y);
    if (!one.empty()) {
      has_path[0] = true;
      has_match[0] = true;
    }
  }

  if ((GR_BLOCK == Grid::stateAt(target_for_third, y)) &&
      (Grid::flavorAt(target_for_third, y) == current_flavor) &&
      (second != target_for_third)) {
    has_match[1] = true;
  } else {
    int two_swap_x, two_swap_y;
    if (has_path[0]) {
      two_swap_x = one[one.size() - 1].current_x;
      two_swap_y = one[one.size() - 1].current_y;
    } else {
      two_swap_x = swap_x;
      two_swap_y = swap_y;
    }
    two = swap_between(
        two_swap_x, two_swap_y,
        third, target_for_third, y);
    if (!two.empty()) {
      has_path[1] = true;
      has_match[1] = true;
    }
  }

  if (has_match[0] && has_match[1]) {
    if (has_path[0])
      ret_path.insert(ret_path.end(), one.begin(), one.end());
    if (has_path[1])
      ret_path.insert(ret_path.end(), two.begin(), two.end());
    ComboAccounting ca;
    ca.combo_start.push_back(std::make_pair(first, y));
    ca.combo_start.push_back(std::make_pair(second, y));
    ca.combo_start.push_back(std::make_pair(third, y));
    ca.combo_end.push_back(std::make_pair(first, y));
    ca.combo_end.push_back(std::make_pair(first + 1, y));
    ca.combo_end.push_back(std::make_pair(first + 2, y));
    for (size_t i = 0; i < ret_path.size(); ++i) {
      ret_path[i].accounting = ca;
    }

    ostringstream r;
    r << ret_path.size() << ret_path;
    MESSAGE("Found: " << r.str());
  }

  return ret_path;
}

static Paths path_for_top_horizontal_combo(int swap_x, int swap_y)
{
  Paths paths;
  for (int y = Grid::top_occupied_row; y >= 1; --y) {
    std::vector<int> combo_flavors = row_threshold_flavors(y, 3);
    if (combo_flavors.empty()) {
      continue;
    }
    for (size_t i = 0; i < combo_flavors.size(); ++i) {
      std::vector<int> locations = row_flavors(y, combo_flavors[i]);
      assert(locations.size() >= 3);

      Paths local_paths;
      const size_t options_max = 3;
      int options[options_max][3] =
        { {0, 1, 2},
          {1, 0, 2},
          {2, 0, 1} };
      for (size_t idx = 0; idx < options_max; ++idx) {
        Path path = generate_horizontal_swap_path(
          swap_x, swap_y,
          locations[options[idx][0]],
          locations[options[idx][1]],
          locations[options[idx][2]],
          y);
        if (!path.empty())
          local_paths.push_back(path);
      }

      Path shortest_path = choose_shortest_path(local_paths);
      if (!shortest_path.empty())
        paths.push_back(shortest_path);
    }
  }

  return paths;
}

static Paths path_for_top_vertical_combo(int swap_x, int swap_y)
{
  Paths paths;
  for (int y = Grid::top_occupied_row; y >= 3; --y) {
    Path ret_path;
    for (int x = 0; x < GC_PLAY_WIDTH; ++x) {
      if (GR_BLOCK == Grid::stateAt(x, y)) {
        int current_flavor = Grid::flavorAt(x, y);
        int first_combo_x, second_combo_x;
        bool has_path[2] = {false, false}, has_match[2] = {false, false};
        Path one_down, two_down;

        if ((GR_BLOCK == Grid::stateAt(x, y - 1)) &&
            (Grid::flavorAt(x, y - 1) == current_flavor)) {
          has_match[0] = true;
          first_combo_x = x;
        } else {
          one_down =  gravity_flavor_path(swap_x, swap_y, current_flavor, x, y - 1, first_combo_x);
          if (!one_down.empty()) {
            has_path[0] = true;
            has_match[0] = true;
          }
        }

        if ((GR_BLOCK == Grid::stateAt(x, y - 2)) &&
            (Grid::flavorAt(x, y - 2) == current_flavor)) {
          has_match[1] = true;
          second_combo_x = x;
        } else {
          int two_down_x, two_down_y;
          if (has_path[0]) {
            two_down_x = one_down[one_down.size()-1].current_x;
            two_down_y = one_down[one_down.size()-1].current_y;
          } else {
            two_down_x = swap_x;
            two_down_y = swap_y;
          }
          two_down = gravity_flavor_path(
              two_down_x, two_down_y,
              current_flavor,
              x, y - 2, second_combo_x);
          if (!two_down.empty()) {
            has_path[1] = true;
            has_match[1] = true;
          }
        }
        if (has_match[0] && has_match[1]) {
          if (has_path[0])
            ret_path.insert(ret_path.end(), one_down.begin(), one_down.end());
          if (has_path[1])
            ret_path.insert(ret_path.end(), two_down.begin(), two_down.end());
          ComboAccounting ca;
          ca.combo_start.push_back(std::make_pair(x, y));
          ca.combo_start.push_back(std::make_pair(first_combo_x, y - 1));
          ca.combo_start.push_back(std::make_pair(second_combo_x, y - 2));
          ca.combo_end.push_back(std::make_pair(x, y));
          ca.combo_end.push_back(std::make_pair(x, y - 1));
          ca.combo_end.push_back(std::make_pair(x, y - 2));
          for (size_t i = 0; i < ret_path.size(); ++i) {
            ret_path[i].accounting = ca;
          }
          paths.push_back(ret_path);
        }
      }
    }
  }

  return paths;
}

static Path choose_from_paths(Paths paths)
{
  return choose_shortest_path(paths);
}

static void path_all_for_flavor(std::vector< PathPortion > &my_path, int hunting_for_flavor)
{
  int move_delay = GC_MOVE_DELAY - 1;
  int swap_x = Swapper::x, swap_y = Swapper::y;

  if (!my_path.empty())
  {
    PathPortion p = my_path[my_path.size() - 1];
    swap_x = p.target_x;
    swap_y = p.target_y;
  }

  for (int x = 0; x < GC_PLAY_WIDTH; ++x) {
    for (int y = 1; y < (Grid::top_effective_row + 1); ++y) {
      if (GR_BLOCK == Grid::stateAt(x, y)) {
        if (Grid::flavorAt(x, y) == hunting_for_flavor) {
          std::vector< PathPortion > additional_path = path_between(
              swap_x, swap_y,
              x, y,
              move_delay);
          if (!additional_path.empty())
            my_path.insert(my_path.end(), additional_path.begin(), additional_path.end());
          swap_x = x; swap_y = y;
          if (swap_x > GC_PLAY_WIDTH - 2) {
            swap_x = GC_PLAY_WIDTH - 2;
          }
        }
      }
    }
  }
}

void ComputerPlayer::gameStart()
{
  /*
  if (!(MetaState::mode & CM_AI))
    return;
  */

  /*
  if ((MetaState::mode & CM_AI_EASY))
    ai = new EasyAI();
  if ((MetaState::mode & CM_AI_MEDIUM))
    ai = new MediumAI();
  if ((MetaState::mode & CM_AI_HARD))
    ai = new HardAI();
  */
  ai = new EasyAI();

  assert(ai != NULL);
  
  start_time = Game::time_step;
  alarm = start_time + GC_MOVE_DELAY;
  lost = false;
  bool all_blocks = true;
  for (int y = Grid::top_effective_row; y >= 3; --y) {
    for (int x = 0; x < GC_PLAY_WIDTH; ++x) {
      if (GR_BLOCK != Grid::stateAt(x, y)) {
        all_blocks = false;
        break;
      }
    }
  }
  //path.push_back(std::make_pair(50, GC_LEFT_KEY));
  /*
  for (int i = 0; i < 5; ++i) {
    path_all_for_flavor(path, i);
  }
  */
  int swap_x = Swapper::x, swap_y = Swapper::y;

  Paths paths = path_for_top_horizontal_combo(swap_x, swap_y);
  Path additional_path = choose_from_paths(paths);
  if (additional_path.empty()) {
    Paths paths = path_for_top_vertical_combo(swap_x, swap_y);
    additional_path = choose_from_paths(paths);
  }
  if (!additional_path.empty())
    path.insert(path.end(), additional_path.begin(), additional_path.end());

  assert(!path.empty());
  alarm = start_time + path[0].alarm;
}

int ComputerPlayer::gameFinish()
{
  return ai->determineLoss() ? GS_WON : GS_LOST;
}

void ComputerPlayer::timeStep()
{
  static bool need_key_up = false;
  /*
  if (!(MetaState::mode & CM_AI))
    return;
  if (!ai) {
    return;
  }
  */

  
  if (Game::time_step >= alarm && alarm != -1) {
    char lame[255];
    if (!need_key_up) {
      assert(!path.empty());
      snprintf(lame, 255, "Executing %d on alarm %d at step %d (%d,%d)",
          path[0].key_action,
          alarm,
          Game::time_step,
          Swapper::x,
          Swapper::y);
      MESSAGE(lame);
      Controller::keyboardPlay(path[0].key_action, 0, 0);
      alarm = Game::time_step + 1;
      need_key_up = true;
    } else {
      assert(!path.empty());
      Controller::keyboardUpPlay(path[0].key_action, 0, 0);
      need_key_up = false;
      path.erase(path.begin());
      if (!path.empty()) {
        alarm = alarm + path[0].alarm;
      } else {
        snprintf(lame, 255, "Path is empty. No new alarm to set (%d,%d)",
            Swapper::x,
            Swapper::y);
        MESSAGE(lame);
        int swap_x = Swapper::x, swap_y = Swapper::y;
        int randomChoice = Random::number(6);
        Paths additional_paths;
        Path additional_path;
        if (randomChoice < 3) {
          MESSAGE("Trying horz");
          additional_paths = path_for_top_horizontal_combo(swap_x, swap_y);
          additional_path = choose_from_paths(additional_paths);
        } else {
          MESSAGE("Trying vertical");
          additional_paths = path_for_top_vertical_combo(swap_x, swap_y);
          additional_path = choose_from_paths(additional_paths);
        }
        if (additional_path.empty()) {
          MESSAGE("Failed.");
          if (randomChoice >= 3) {
            MESSAGE("Trying horz");
            additional_paths = path_for_top_horizontal_combo(swap_x, swap_y);
            additional_path = choose_from_paths(additional_paths);
          } else {
            MESSAGE("Trying vertical");
            additional_paths = path_for_top_vertical_combo(swap_x, swap_y);
            additional_path = choose_from_paths(additional_paths);
          }
        }
        if (!additional_path.empty()) {
          path.insert(path.end(), additional_path.begin(), additional_path.end());

        } else {
          PathPortion p;
          p.alarm = GC_MOVE_DELAY;
          p.key_action = GC_ADVANCE_KEY;
          path.push_back(p);
        }

        if (!path.empty()) {
          alarm = alarm + path[0].alarm;
        } else {
          alarm = -1;
        }
      }
    }
  }

  // handle the lights
  LevelLights::handleAI();

  /*
  ComputerPlayerAI &localAi = *ai;
  if (first_time) {
    MESSAGE("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
    LOG("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
    first_time = false;
  }
  if (Game::time_step >= localAi.alarm()) {
    localAi.garbageAmount()->sendToGenerator();
#ifndef NDEBUG
    std::cout << "init pop: " << GC_INITIAL_POP_DELAY << std::endl;
    std::cout << "steps per second: " << GC_STEPS_PER_SECOND << std::endl;
    std::cout << "Height: " << ai->garbageQueue()->height() << std::endl;
#endif
    localAi.resetAlarm();
    MESSAGE("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
    LOG("AI will drop again in " << ((localAi.alarm() - Game::time_step) / GC_STEPS_PER_SECOND) << " seconds");
  }
  if(localAi.determineLoss()) {
    Game::aiPlayerLoss();
  }
  */
}

void ComputerPlayer::addGarbage ( int height, int width, int flavor ) {
  assert(ai != NULL);
  MESSAGE("Adding garbage to queue");
  ai->garbageQueue()->add(height, width, flavor);
  _impact = true;
}

bool ComputerPlayer::checkLevelLightDying()
{
  int height = ai->garbageQueue()->height();
  int ninety = (int)(ai->lossHeight() * .9);
  if (ninety == ai->lossHeight())
    ninety = ai->lossHeight() - 1;
  if (height >= ninety)
    return true;
  return false;
}

double ComputerPlayer::lightPartition (int n)
{
  static double max = LL_NUMBER_LEVEL_LIGHTS;
  double lh = ai->lossHeight();
  double partition = lh / max;
  double colorh = n * partition;
  return colorh;
}

bool ComputerPlayer::checkLevelLightBlue(int n)
{
  if (lightPartition(n) >= ai->garbageQueue()->height())
    return true;
  else
    return false;
}

int ComputerPlayer::findTopRed()
{
  for (int i=0; i < LL_NUMBER_LEVEL_LIGHTS; ++i) {
    if (lightPartition(i) >= ai->garbageQueue()->height())
      return i;
  }
  return 0;
}

int ComputerPlayer::levelLightImpact (  )
{
  if (impact(true))
    return findTopRed();

  return 0;
}

bool ComputerPlayer::impact (bool reset)
{
  bool ret = _impact;
  if (reset) _impact = false;
  return ret;
}

void ComputerPlayer::shiftUp (  )
{
  for (size_t i = 0; i < path.size(); ++i) {
    path[i].shiftUp();
  }
}
