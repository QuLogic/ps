
#' List of 'errno' error codes
#'
#' For the errors that are not used on the current platform, `value` is
#' `NA_integer_`.
#'
#' A data frame with columns: `name`, `value`, `description`.
#' @export
#' @examples errno()

errno <- function() {
  err <- as.list(ps_env$constants$errno)
  err <- err[order(names(err))]
  data.frame(
    stringsAsFactors = FALSE,
    row.names = NULL,
    name = names(err),
    value = vapply(err, "[[", integer(1), 1),
    description = vapply(err, "[[", character(1), 2)
  )
}
