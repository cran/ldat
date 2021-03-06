% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/ldat.R
\name{ldat}
\alias{ldat}
\alias{is_ldat}
\title{Create an ldat object}
\usage{
ldat(...)

is_ldat(x)
}
\arguments{
\item{...}{these arguments are of either the form `tag = value' or `value'. 
Each argument becomes a column in the \code{ldat}. All columns are 
required to have the same length.}

\item{x}{object for which to check if it is of type ldat}
}
\value{
An object of type \code{ldat}. This object is basically a list with 
\code{\link{lvec}} objects.
}
\description{
This function creates an \code{ldat} object, which behaves similar to a 
\code{data.frame} except that its columns are \code{\link{lvec}}. This 
allows and \code{ldat} to have an arbitrary large number of rows without
running into memory problems.
}
\details{
Each of the arguments of \code{ldat} is converted to an \code{\link{lvec}} 
when it isn't already and \code{\link{lvec}} using calls to 
\code{\link{as_lvec}}. The arguments are required to all have the same 
length (unlike \code{\link{data.frame}}).
}
\examples{
# Create ldat object from r-objects
a <- ldat(id = 1:20, x = letters[1:20], y = rnorm(20))
# this is identical to
a <- ldat(id = as_lvec(1:20), x = as_lvec(letters[1:20]), 
    y = as_lvec(rnorm(20)))

}
