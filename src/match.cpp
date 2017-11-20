#include <lvec_interface.h>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iostream>
#include <iomanip>
#include "r_export.h"

std::ostream& operator<<(std::ostream& str, const cppr::boolean& vec) {
  int val = vec;
  str << "bool: " << val;
  return str;
}

class match_visitor : public ldat::lvec_visitor {
  public:
    match_visitor(ldat::vec* order, ldat::vec* tab, ldat::vec* order_tab, bool na_incomparable = false) : 
        order_(order), tab_(tab), order_tab_(order_tab), result_(0), 
        na_incomparable_(na_incomparable) {
    }

    template<class T>
    class compare_lt {
      public:
        bool operator()(const T& lhs, const T& rhs) {
          if (cppr::is_nan(lhs)) return false;
          if (cppr::is_nan(rhs)) return true;
          return lhs < rhs;
        }
    };

    template<class T>
    class compare_eq {
      public:
        bool operator()(const T& lhs, const T& rhs, bool na_incomparable) {
          // we need the manually handle missing values, because for double na's
          // are coded as nan and therefore regular comparison doesn't work.
          if (na_incomparable) {
            if (cppr::is_nan(lhs) || cppr::is_nan(rhs)) return false;
          } else {
            if (cppr::is_nan(lhs) && cppr::is_nan(rhs)) return true;
            if (cppr::is_nan(lhs) || cppr::is_nan(rhs)) return false;
          }
          return lhs == rhs;
        }
    };

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
      ldat::vec::vecsize size = vec.size();
      std::unique_ptr<ldat::lvec<double> > result(new ldat::lvec<double>(size));

      compare_lt<T> less_than;
      compare_eq<T> equal;
      
      if (size > 0 && tab_->size() > 0) {
      
        ldat::vec::vecsize j = 0;
        ldat::vec::vecsize index_tab = order_tab_->get_of_type(j, double())-1.0;
        T el_tab = tab_->get_of_type(index_tab, cppr::base_type(T()));
        for (ldat::vec::vecsize i = 0; i != size; ++i) {
          ldat::vec::vecsize index_vec = order_->get_of_type(i, double())-1.0;
          T el = vec.get(index_vec);
          while (less_than(el_tab, el) && j < (tab_->size()-1)) {
            ++j;
            index_tab = order_tab_->get_of_type(j, double())-1.0;
            el_tab = tab_->get_of_type(index_tab, cppr::base_type(T()));
  
          }
          if (equal(el, el_tab, na_incomparable_)) {
            result->set(index_vec, index_tab + 1.0);
          } else {
            result->set(index_vec, cppr::na<double>());
          }
        }
      }
      
      // copy temporary result to result
      if (result_) delete result_;
      result_ = result.release();
    }

    void visit(ldat::lvec<double>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<int>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<cppr::boolean>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<std::string>& vec) {
      return visit_template(vec);
    }

    ldat::vec* result() {
      return result_;
    }

  private:
    ldat::vec* order_;
    ldat::vec* tab_;
    ldat::vec* order_tab_;
    ldat::lvec<double>* result_;
    bool na_incomparable_;
};

extern "C" {
  SEXP lmatch(SEXP rv, SEXP rvo, SEXP rtab, SEXP rtabo, SEXP rna_incomp) {
    CPPRTRY
    ldat::vec* v = sexp_to_vec(rv);
    ldat::vec* vo = sexp_to_vec(rvo);
    if (v->size() != vo->size()) 
      throw std::runtime_error("Lengths of vector and order of vector are unequal.");
    // check and convert table
    ldat::vec* tab = sexp_to_vec(rtab);
    ldat::vec* tabo = sexp_to_vec(rtabo);
    if (tab->size() != tabo->size()) 
      throw std::runtime_error("Lengths of table and order of table are unequal.");
    cppr::rvec<cppr::logical> na_incomp{rna_incomp};
    if (na_incomp.length() != 1) 
      throw std::runtime_error("na_incomparable should be a logical vector of length 1.");
    // call visitor
    match_visitor visitor(vo, tab, tabo, na_incomp[0]);
    v->visit(&visitor);
    return vec_to_sexp(visitor.result());
    CPPRCATCH
  }
}

