#include <AMReX.H>
#include <AMReX_Box.H>
#include <AMReX_IntVect.H>
#include <AMReX_Array4.H>
#include <AMReX_FArrayBox.H>

using namespace amrex;

void test ();

int main(int argc, char* argv[])
{
    amrex::Initialize(argc,argv);
    test();
    amrex::Finalize();
}

template <typename T>
void func (amrex::Array4<amrex::Real> const& arr, const amrex::Box& box, double val);  // note mismatch with definition below

template <typename T>
void func (amrex::Array4<amrex::Real> const& arr, const amrex::Box& box, const double val) {
    amrex::ParallelFor(box,
                       [=] AMREX_GPU_DEVICE (int i, int j, int k) noexcept
                       {
                           arr(i, j, k) = val;
                       });
}

struct ProblemType {};

void test ()
{
    amrex::Box box(amrex::IntVect(AMREX_D_DECL(0, 0, 0)),
                   amrex::IntVect(AMREX_D_DECL(127, 127, 127)));
    amrex::FArrayBox fab(box, 1);
    auto arr = fab.array();
    func<ProblemType>(arr, box, 1.0);
}
