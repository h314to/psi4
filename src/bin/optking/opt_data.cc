/*! \file    opt_data.cc
    \ingroup OPT10
    \brief   funtions for working with optimization data 
*/

#include "opt_data.h"

#define EXTERN
#include "globals.h"

namespace opt {

//minimal constructor - just allocates memory
STEP_DATA::STEP_DATA(int Nintco, int Ncart) {
  f_q  = init_array(Nintco);
  geom = init_array(Ncart);
  energy = 0.0;
  DE_predicted = 0.0;
  unit_step = init_array(Nintco);
  dq_norm = 0.0;
  dq_gradient = 0.0;
  dq_hessian = 0.0;
  dq = init_array(Nintco);
}

//free memory
STEP_DATA::~STEP_DATA() {
  free_array(f_q);
  free_array(geom);
  free_array(unit_step);
  free_array(dq);
}

// read entry from binary file ; file pointer must be in right place for qchem code
#if defined(PSI4)
void STEP_DATA::read(int istep, int Nintco, int Ncart) {
  char lbl[80];
  using namespace psi;
printf("Reading step data\n");
  sprintf(lbl, "f_q %d", istep);
  psio_read_entry(PSIF_OPTKING, lbl, (char *) f_q,    Nintco*sizeof(double));
  sprintf(lbl, "geom %d", istep);
  psio_read_entry(PSIF_OPTKING, lbl, (char *) geom,    Ncart*sizeof(double));
  sprintf(lbl, "energy %d", istep);
  psio_read_entry(PSIF_OPTKING, lbl, (char *) &energy,       sizeof(double));
  sprintf(lbl, "DE_predicted %d", istep);
  psio_read_entry(PSIF_OPTKING, lbl, (char *) &DE_predicted, sizeof(double));
  sprintf(lbl, "unit_step %d", istep);
  psio_read_entry(PSIF_OPTKING, lbl, (char *) unit_step, Nintco*sizeof(double));
  sprintf(lbl, "dq_norm %d", istep);
  psio_read_entry(PSIF_OPTKING, lbl, (char *) &dq_norm,      sizeof(double));
  sprintf(lbl, "dq_gradient %d", istep);
  psio_read_entry(PSIF_OPTKING, lbl, (char *) &dq_gradient,  sizeof(double));
  sprintf(lbl, "dq_hessian %d", istep);
  psio_read_entry(PSIF_OPTKING, lbl, (char *) &dq_hessian,   sizeof(double));
  sprintf(lbl, "dq %d", istep);
  psio_read_entry(PSIF_OPTKING, lbl, (char *) dq,       Nintco*sizeof(double));
}
#elif defined (QCHEM4)
void STEP_DATA::read(ifstream & fin, int Nintco, int Ncart) {
  fin.read( (char *) f_q,    Nintco*sizeof(double));
  fin.read( (char *) geom,    Ncart*sizeof(double));
  fin.read( (char *) &energy,       sizeof(double));
  fin.read( (char *) &DE_predicted, sizeof(double));
  fin.read( (char *) unit_step, Nintco*sizeof(double));
  fin.read( (char *) &dq_norm,      sizeof(double));
  fin.read( (char *) &dq_gradient,  sizeof(double));
  fin.read( (char *) &dq_hessian,   sizeof(double));
  fin.read( (char *) dq,       Nintco*sizeof(double));
}
#endif

// save geometry and energy
void STEP_DATA::save_geom_energy(double *geom_in, double energy_in, int Ncart) {
  array_copy(geom_in, geom, Ncart);
  energy = energy_in;
}

// save rest of stuff
void STEP_DATA::save_step_info(double DE_predicted_in, double *unit_step_in, double dq_norm_in,
    double dq_gradient_in, double dq_hessian_in, int Nintco) {
  DE_predicted = DE_predicted_in;
  array_copy(unit_step_in, unit_step, Nintco);
  dq_norm = dq_norm_in;
  dq_gradient = dq_gradient_in;
  dq_hessian = dq_hessian_in;
}

// read entry from binary file ; file pointer must be in right place for qchem code
//write entry to binary file
#if defined(PSI4)
void STEP_DATA::write(int istep, int Nintco, int Ncart) {
  char lbl[80];
  using namespace psi;
printf("Write step data\n");
  fprintf(outfile,"Writing step data to psio file.\n");
  sprintf(lbl, "f_q %d", istep);
  psio_write_entry(PSI_OPTDATA_FILE_NUM, lbl, (char *) f_q, Nintco*sizeof(double));
  sprintf(lbl, "geom %d", istep);
  psio_write_entry(PSI_OPTDATA_FILE_NUM, lbl, (char *) geom, Ncart*sizeof(double));
  sprintf(lbl, "energy %d", istep);
  psio_write_entry(PSI_OPTDATA_FILE_NUM, lbl, (char *) &energy, sizeof(double));
  sprintf(lbl, "DE_predicted %d", istep);
  psio_write_entry(PSI_OPTDATA_FILE_NUM, lbl, (char *) &DE_predicted, sizeof(double));
  sprintf(lbl, "unit_step %d", istep);
  psio_write_entry(PSI_OPTDATA_FILE_NUM, lbl, (char *) unit_step, Nintco*sizeof(double));
  sprintf(lbl, "dq_norm %d", istep);
  psio_write_entry(PSI_OPTDATA_FILE_NUM, lbl, (char *) &dq_norm, sizeof(double));
  sprintf(lbl, "dq_gradient %d", istep);
  psio_write_entry(PSI_OPTDATA_FILE_NUM, lbl, (char *) &dq_gradient, sizeof(double));
  sprintf(lbl, "dq_hessian %d", istep);
  psio_write_entry(PSI_OPTDATA_FILE_NUM, lbl, (char *) &dq_hessian, sizeof(double));
  sprintf(lbl, "dq %d", istep);
  psio_write_entry(PSI_OPTDATA_FILE_NUM, lbl, (char *) dq, Nintco*sizeof(double));
}
#elif defined(QCHEM4)
void STEP_DATA::write(ofstream & fout, int Nintco, int Ncart) {
printf("Write step data\n");
  fout.write( (char *) f_q,    Nintco*sizeof(double));
  fout.write( (char *) geom,    Ncart*sizeof(double));
  fout.write( (char *) &energy,       sizeof(double));
  fout.write( (char *) &DE_predicted, sizeof(double));
  fout.write( (char *) unit_step, Nintco*sizeof(double));
  fout.write( (char *) &dq_norm,      sizeof(double));
  fout.write( (char *) &dq_gradient,  sizeof(double));
  fout.write( (char *) &dq_hessian,   sizeof(double));
  fout.write( (char *) dq,       Nintco*sizeof(double));
}
#endif

// constructor function reads available data and allocates memory for current step
OPT_DATA::OPT_DATA(int Nintco_in, int Ncart_in) {
  Nintco = Nintco_in;
  Ncart = Ncart_in;
  H = init_matrix(Nintco, Nintco);
  rfo_eigenvector = init_array(Nintco+1);
  bool data_file_present = false;

// determine if old data file is present
#if defined(PSI4)
  using namespace psi;
  //if (!psio_open_check(PSIF_OPTKING))
  psio_open(PSIF_OPTKING, PSIO_OPEN_OLD);
  if (psio_rd_toclen(PSIF_OPTKING) > 0)
    data_file_present = true;
  psio_close(PSIF_OPTKING, 1);
#elif defined(QCHEM4)
  ifstream fin(FILENAME_OPT_DATA, ios_base::in | ios_base::binary);
  data_file_present = fin.is_open();
  fin.close();
#endif

printf("Read opt data\n");

  if (!data_file_present) { 
    fprintf(outfile, "\tPrevious optimization step data not found.  Starting new optimization.\n");
printf("Previous data not found.\n");
    iteration = 0;
    consecutive_back_steps = 0;
  }
  else { // previous file exists
    int Nintco_old, Ncart_old;
#if defined (PSI4)
    psio_open(PSIF_OPTKING, PSIO_OPEN_OLD);
    psio_read_entry(PSIF_OPTKING, "Nintco", (char *) &Nintco_old, sizeof(int));
    psio_read_entry(PSIF_OPTKING, "Ncart",  (char *) &Ncart_old, sizeof(int));
#elif defined(QCHEM4)
    ifstream fin(FILENAME_OPT_DATA, ios_base::in | ios_base::binary);
    fin.read( (char *) &Nintco_old, sizeof(int));
    fin.read( (char *)  &Ncart_old, sizeof(int));
#endif
    if (Nintco_old != Nintco)
      fprintf(outfile, "\tThe number of coordinates has changed.  Ignoring old data.\n");
    if (Ncart_old != Ncart)
      fprintf(outfile, "\tThe number of atoms has changed.  Ignoring old data.\n");

    if ( (Nintco_old != Nintco) || (Ncart_old != Ncart) ) { // don't use old data
printf("Ignoring old data\n");
      iteration = 0;
      consecutive_back_steps = 0;
      // delete old data
#if defined (PSI4)
      psio_close(PSIF_OPTKING, 0);
#elif defined (QCHEM4)
      fin.close();
      std::remove(FILENAME_OPT_DATA);
#endif
    }
    else { // read in old optimization data
#if defined (PSI4)
      psio_read_entry(PSIF_OPTKING, "H", (char *) H[0], Nintco * Nintco * sizeof(double) );
      psio_read_entry(PSIF_OPTKING, "iteration", (char *) &iteration, sizeof(int));
      psio_read_entry(PSIF_OPTKING, "consecutive_back_steps", (char *) &consecutive_back_steps, sizeof(int));
      psio_read_entry(PSIF_OPTKING, "rfo_eigenvector", (char *) rfo_eigenvector, (Nintco+1)*sizeof(double));
      for (int i=0; i<iteration; ++i) {
        STEP_DATA *one_step = new STEP_DATA(Nintco, Ncart);
        one_step->read(i+1, Nintco, Ncart);
        steps.push_back(one_step);
      }
      psio_close(PSIF_OPTKING, 1);
#elif defined (QCHEM4)
      fin.read( (char *) H[0], Nintco * Nintco * sizeof(double) );
      fin.read( (char *) &iteration, sizeof(int));
      fin.read( (char *) &consecutive_back_steps, sizeof(int));
      fin.read( (char *) rfo_eigenvector, (Nintco+1)*sizeof(double));
      for (int i=0; i<iteration; ++i) {
        STEP_DATA *one_step = new STEP_DATA(Nintco, Ncart);
        one_step.read(fin, Nintco, Ncart);
        steps.push_back(one_step);
      }
      fin.close();
#endif
    }
  }

  ++iteration; // increment for current step
  // create memory for this, current step
  STEP_DATA *one_step = new STEP_DATA(Nintco, Ncart);
  steps.push_back(one_step);
}

OPT_DATA::~OPT_DATA() {
  free_matrix(H);
  free_array(rfo_eigenvector);
  for (int i=0; i<steps.size(); ++i)
    delete steps[i];
  steps.clear();
}

// write data to binary file
void OPT_DATA::write(void) {
#if defined (PSI4)
  using namespace psi;
  psio_open(PSIF_OPTKING, PSIO_OPEN_OLD);

printf("Write opt data\n");

  fprintf(outfile,"Writing optimization data to psio file.\n");
  psio_write_entry(PSIF_OPTKING, "Nintco", (char *) &Nintco, sizeof(int));
  psio_write_entry(PSIF_OPTKING, "Ncart" , (char *) &Ncart , sizeof(int));
  psio_write_entry(PSIF_OPTKING, "H", (char *) H[0], Nintco * Nintco * sizeof(double) );
  psio_write_entry(PSIF_OPTKING, "iteration", (char *) &iteration, sizeof(int));
  psio_write_entry(PSIF_OPTKING, "consecutive_back_steps", (char *) &consecutive_back_steps, sizeof(int));
  psio_write_entry(PSIF_OPTKING, "rfo_eigenvector", (char *) rfo_eigenvector, (Nintco+1)*sizeof(double));

  for (int i=0; i<steps.size(); ++i)
    steps[i]->write(i+1, Nintco, Ncart);
  psio_close(PSIF_OPTKING, 1);

#elif defined (QCHEM4) 
  ofstream fout(FILENAME_OPT_DATA, ios_base::out | ios_base::trunc | ios_base::binary);
  if (!fout.is_open())
    throw("Could not open file to save geometry data.\n");

  fout.write( (char *) &Nintco, sizeof(int));
  fout.write( (char *) &Ncart,  sizeof(int));
  fout.write( (char *) H[0], Nintco * Nintco * sizeof(double) );
  fout.write( (char *) &iteration, sizeof(int));
  fout.write( (char *) &consecutive_back_steps, sizeof(int));
  fout.write( (char *) rfo_eigenvector, (Nintco+1)*sizeof(double));

  for (int i=0; i<steps.size(); ++i) 
    steps[i]->write(fout, Nintco, Ncart);
  fout.close();
#endif
}


// Check convergence criteria and print status to output file
// return true, if geometry is optimized
// Checks maximum force && (Delta(E) || maximum disp)
bool OPT_DATA::conv_check(void) const {
  // get this step info
  double *f =  g_forces_pointer();
  double max_force = array_abs_max(f, Nintco);

  double *dq =  g_dq_pointer();
  double max_disp = array_abs_max(dq, Nintco);

  double DE;
  if (g_iteration() > 1) DE = g_energy() - g_last_energy();
  else DE = g_energy();

  fprintf(outfile, "\n\tConvergence Check Cycle %4d:\n", iteration);
  fprintf(outfile, "\t                    Actual        Tolerance     Converged?\n");
  fprintf(outfile, "\t----------------------------------------------------------\n");

  if ( fabs(Opt_params.conv_max_force) < 1.0e-15 ) fprintf(outfile, "\tMAX Force        %10.1e\n", max_force);
  else fprintf(outfile, "\tMAX Force        %10.1e %14.1e %11s\n", max_force, Opt_params.conv_max_force,
       ((max_force < Opt_params.conv_max_force) ? "yes" : "no"));

  if ( fabs(Opt_params.conv_max_DE) < 1.0e-15 ) fprintf(outfile, "\tEnergy Change    %10.1e\n", fabs(DE));
  else fprintf(outfile, "\tEnergy Change    %10.1e %14.1e %11s\n", DE, Opt_params.conv_max_DE,
       ((fabs(DE) < Opt_params.conv_max_DE) ? "yes" : "no"));

  if ( fabs(Opt_params.conv_max_disp) < 1.0e-15 ) fprintf(outfile, "\tMAX Displacement %10.1e\n", max_disp);
  else fprintf(outfile, "\tMAX Displacement %10.1e %14.1e %11s\n", max_disp, Opt_params.conv_max_disp,
       ((max_disp < Opt_params.conv_max_disp) ? "yes" : "no"));

  fprintf(outfile, "\t----------------------------------------------------------\n");

  printf("\tMAX Force %10.1e : Energy Change %10.1e : MAX Displacment %10.1e\n", max_force, DE, max_disp);

  if ((max_force < Opt_params.conv_max_force) &&
       ((fabs(DE) < Opt_params.conv_max_DE) || (max_disp < Opt_params.conv_max_disp)))  {
    return true; // structure is optimized!
  }
  else 
    return false;
}

void OPT_DATA::summary(void) const {
  double DE, *f, *dq, max_force, max_disp;

  fprintf(outfile,"\n\t            ****  Optimization Summary  ****\n");
  fprintf(outfile,"\t----------------------------------------------------------------------------\n");
  fprintf(outfile,"\t Step         Energy             Delta(E)      MAX force    MAX Delta(q)  \n");
  fprintf(outfile,"\t----------------------------------------------------------------------------\n");

  for (int i=0; i<iteration; ++i) {

    if (i == 0) DE = g_energy(i);
    else DE = g_energy(i) - g_energy(i-1); 

    f =  g_forces_pointer(i);
    max_force = array_abs_max(f, Nintco);

    dq =  g_dq_pointer(i);
    max_disp = array_abs_max(f, Nintco);

    fprintf(outfile,"\t %3d  %18.12lf  %18.12lf  %10.2e   %10.2e\n", i+1, g_energy(i),
      DE, max_force, max_disp);

  }
  fprintf(outfile,"\t----------------------------------------------------------------------\n");
  fprintf(outfile,"\n");
}

// do hessian update
void OPT_DATA::H_update(opt::MOLECULE & mol) {

  if (Opt_params.H_update == OPT_PARAMS::BFGS)
    fprintf(outfile,"\n\tPerforming BFGS update");
  else if (Opt_params.H_update == OPT_PARAMS::MS)
    fprintf(outfile,"\n\tPerforming Murtagh/Sargent update");
  else if (Opt_params.H_update == OPT_PARAMS::POWELL)
    fprintf(outfile,"\n\tPerforming Powell update");
  else if (Opt_params.H_update == OPT_PARAMS::BOFILL)
    fprintf(outfile,"\n\tPerforming Bofill update");
  else if (Opt_params.H_update == OPT_PARAMS::NONE) {
    fprintf(outfile,"\n\tNo Hessian update performed.\n");
    return;
  }

  int istep, step_start;
  int step_this = steps.size()-1;

  /*** Read/compute current internals and forces ***/
  double *f, *x, *q;
  f = steps[step_this]->g_forces_pointer();
  x = steps[step_this]->g_geom_pointer();

  mol.set_geom_array(x);
  //mol.compute_intco_values();
  q = mol.intco_values();
  mol.fix_tors_near_180(); // fix configuration for torsions

  if (Opt_params.H_update_use_last == 0) { // use all available old gradients
    step_start = 0;
  }
  else {
    step_start = steps.size() - 1 - Opt_params.H_update_use_last;
    if (step_start < 0) step_start = 0;
  }
  fprintf(outfile," with previous %d gradient(s).\n", step_this-step_start);

  int i, j, i_step;
  double *f_old, *x_old, *q_old, *dq, *dg;
  double gq, qq, qz, zz, phi;

  double **H = g_H_pointer();
  double **H_new = init_matrix(Nintco, Nintco);

  dq = init_array(Nintco);
  dg = init_array(Nintco);

  for (i_step=step_start; i_step < step_this; ++i_step) {

    // Read/compute old internals and forces
    f_old = g_forces_pointer(i_step);
    x_old = g_geom_pointer(i_step);

    mol.set_geom_array(x_old);
    //mol.compute_intco_values(); // (will use torsion fixing above)
    q_old = mol.intco_values();

    for (i=0;i<Nintco;++i) {
      dq[i] = q[i] - q_old[i];
      dg[i] = (-1.0) * (f[i] - f_old[i]); // gradients -- not forces!
    }
    gq = array_dot(dq, dg, Nintco);
    qq = array_dot(dq, dq, Nintco);

    // Schlegel 1987 Ab Initio Methods in Quantum Chemistry 
    // To make formulas work for Hessian, i.e., the 2nd derivatives switch dx and dg
    if (Opt_params.H_update == OPT_PARAMS::BFGS) {
      double **temp_mat, **X;
      // Let a = dg^T.dq and X = (I - dg*dq^T) / a
      // Then H = X * H_old * X^T + dg*dg^T/a .
      X = unit_matrix(Nintco);
      for (i=0; i<Nintco; ++i)
        for (j=0; j<Nintco; ++j)
          X[i][j] -= (dg[i] * dq[j]) / gq ;

      temp_mat = init_matrix(Nintco,Nintco);
      opt_matrix_mult(X, 0, H, 0, temp_mat, 0, Nintco, Nintco, Nintco, 0);
      opt_matrix_mult(temp_mat, 0, X, 1, H_new, 0, Nintco, Nintco, Nintco, 0);

      for (i=0; i<Nintco; ++i)
        for (j=0; j<Nintco; ++j)
          H_new[i][j] += dg[i] * dg[j] / gq ;

      free_matrix(temp_mat);
      free_matrix(X);
    }
    else if (Opt_params.H_update == OPT_PARAMS::MS) {
      // Equations taken from Bofill article below
      double *Z = init_array(Nintco);
      opt_matrix_mult(H, 0, &dq, 1, &Z, 1, Nintco, Nintco, 1, 0);
      for (i=0; i<Nintco; ++i)
        Z[i] = dg[i] - Z[i];

      qz = array_dot(dq, Z, Nintco);

      for (i=0; i<Nintco; ++i)
        for (j=0; j<Nintco; ++j)
          H_new[i][j] = H[i][j] + Z[i] * Z[j] / qz ;

      free_array(Z);
    }
    else if (Opt_params.H_update == OPT_PARAMS::POWELL) {
      // Equations taken from Bofill article below
      double * Z = init_array(Nintco);
      opt_matrix_mult(H, 0, &dq, 1, &Z, 1, Nintco, Nintco, 1, 0);
      for (i=0; i<Nintco; ++i)
        Z[i] = dg[i] - Z[i];

      qz = array_dot(dq, Z, Nintco);

      for (i=0; i<Nintco; ++i)
        for (j=0; j<Nintco; ++j)
          H_new[i][j] = H[i][j] - qz/(qq*qq)*dq[i]*dq[j] + (Z[i]*dq[j] + dq[i]*Z[j])/qq;

      free_array(Z);
    }
    else if (Opt_params.H_update == OPT_PARAMS::BOFILL) {
      /* This functions performs a Bofill update on the Hessian according to
      J. M. Bofill, J. Comp. Chem., Vol. 15, pages 1-11 (1994). */
      // Bofill = (1-phi) * MS + phi * Powell
      double *Z = init_array(Nintco);
      opt_matrix_mult(H, 0, &dq, 1, &Z, 1, Nintco, Nintco, 1, 0);
      for (i=0; i<Nintco; ++i)
        Z[i] = dg[i] - Z[i];

      qz = array_dot(dq, Z, Nintco);
      zz = array_dot(Z, Z, Nintco);

      phi = 1.0 - qz*qz/(qq*zz);
      if (phi < 0.0) phi = 0.0;
      if (phi > 1.0) phi = 1.0;

      for (i=0; i<Nintco; ++i) // (1-phi)*MS
        for (j=0; j<Nintco; ++j)
          H_new[i][j] = H[i][j] + (1.0-phi) * Z[i] * Z[j] / qz ;

      for (i=0; i<Nintco; ++i)
        for (j=0; j<Nintco; ++j) // (phi * Powell)
          H_new[i][j] += phi * (-1.0*qz/(qq*qq)*dq[i]*dq[j] + (Z[i]*dq[j] + dq[i]*Z[j])/qq);
      free_array(Z);
    }
    free_array(q_old);
  }
  free_array(q);
  free_array(dq);
  free_array(dg);

  // put original geometry back into molecule object
  mol.set_geom_array(x);

  // limit allowed changes to Hessian to 0.3 aJ/Ang^2, aJ/rad^2 or 50%
  double max;
  double limit = Opt_params.H_change_limit*_bohr2angstroms*_bohr2angstroms/_hartree2aJ;
  for (i=0; i<Nintco; ++i)
    for (j=0; j<Nintco; ++j)
      H_new[i][j] -= H[i][j]; // compute change in Hessian

  for (i=0; i<Nintco; ++i) {
    for (j=0; j<Nintco; ++j) {
      max = ((0.5*fabs(H[i][j]) > limit) ? (0.5*fabs(H[i][j])) : limit);

      if (fabs(H_new[i][j]) < max)
        H[i][j] += H_new[i][j];
      else
        H[i][j] += limit * H_new[i][j]/fabs(H_new[i][j]);
    }
  }
  if (Opt_params.print_lvl >= 2) {
    fprintf(outfile, "Updated Hessian (in au)\n");
    print_matrix(outfile, H, Nintco, Nintco);
  }
  free_matrix(H_new);

  return;
}

double ** OPT_DATA::read_cartesian_H(void) const {

  double **H_cart = init_matrix(Ncart, Ncart);

  ifstream H_cart_file;
  H_cart_file.open(FILENAME_CARTESIAN_H, ios_base::in);

  try {
    int n;
    H_cart_file >> n; // read natom
    H_cart_file >> n; // read natom*6 (?)

    for (int i=0; i<Ncart; ++i)
      for (int j=0; j<Ncart; ++j)
        H_cart_file >> H_cart[i][j];
  }
  catch (std::ios_base::failure & bf) {
    printf("Error reading cartesian Hessian matrix\n");
    fprintf(outfile,"Error reading cartesian Hessian matrix\n");
    throw "Error reading cartesian Hessian matrix\n";
  }

  H_cart_file.close();

  if (Opt_params.print_lvl >= 3) {
    fprintf(outfile,"\tCartesian Hessian matrix read: \n");
    print_matrix(outfile, H_cart, Ncart, Ncart);
    fflush(outfile);
  }

  return H_cart;
}

} // end ::opt


