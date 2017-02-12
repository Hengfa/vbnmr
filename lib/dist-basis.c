
/* include the basis distribution header. */
#include <vbnmr/dist-basis.h>

/* * * * multivariate expectations: * * * */

/* expect_basis(): compute the expectation of a single basis function term,
 * for a given signal phase, at a given time.
 *
 * arguments:
 *  @mdl: model structure pointer.
 *  @j: signal basis index.
 *  @k: signal phase index.
 *  @l: signal term index.
 *  @t: multidimensional time vector.
 *
 * returns:
 *  E[psi_j^{k,l}(t | omega_j, rho_j)]
 */
inline double
expect_basis (const model_t *mdl, const unsigned int j,
              const unsigned int k, const unsigned int l,
              const vector_t *t) {
  /* compute and return the term expectation. */
  return expect_decay(mdl, j, mdl->D, t) *
         expect_freq(mdl, j, k, l, mdl->D, t);
}

/* * * * multivariate interactions: * * * */

/* interact_basis(): compute the interaction of two signal model basis
 * elements. this is used for populating the interaction matrix during
 * mean-field amplitude updates within the model inference routines.
 *
 * arguments:
 *  @mdl: model structure pointer.
 *  @j1, @j2: signal basis indices.
 *  @l1, @l2: signal term indices.
 *  @k: signal phase index.
 *  @t: multidimensional time vector.
 *
 * returns:
 *  E[psi_j1^{k,l1}(t | omega_j1, rho_j1)
      psi_j2^{k,l2}(t | omega_j2, rho_j2)]
 */
inline double
interact_basis (const model_t *mdl,
                const unsigned int j1, const unsigned int j2,
                const unsigned int l1, const unsigned int l2,
                const unsigned int k, const vector_t *t) {
  /* compute and return the interaction value. */
  return interact_freq(mdl, j1, l1, j2, l2, k, mdl->D, t) *
         interact_decay(mdl, j1, j2, mdl->D, t);
}

/* * * * multivariate covariances: * * * */

/* cov_basis(): compute the covariance function of a single signal
 * model basis element. this is used for gaussian process kernel
 * computation, specifically by model_cov().
 *
 * arguments:
 *  @mdl: pointer to the model structure to access.
 *  @j: signal basis index to compute covariance for.
 *  @k1, @k2: phase pair on which to compute covariance.
 *  @t1, @t2: time points at which to compute covariance.
 *
 * returns:
 *  covariance between the signal basis @j on the phases @k1 and @k2
 *  at times @t1 and @t2.
 *
 *  i.e.:
 *   E[psi_j^{k1}(t1 | omega_j, rho_j)
 *     psi_j^{k2}(t2 | omega_j, rho_j)]
 */
double cov_basis (const model_t *mdl, const unsigned int j,
                  const unsigned int k1, const unsigned int k2,
                  const vector_t *t1, const vector_t *t2) {
  /* @cov: output covariance value. */
  double cov = 0.0;

  /* sum together the modulation terms. */
  for (unsigned int l = 0; l < mdl->K; l++)
    cov += cov_freq(mdl, j, k1, k2, l, l, t1, t2);

  /* finalize and return the computed covariance. */
  cov *= cov_decay(mdl, j, t1, t2);
  return cov;
}

