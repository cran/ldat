% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/slice_range.R
\name{slice_range}
\alias{slice_range}
\alias{slice_range.lvec}
\alias{slice_range.ldat}
\alias{slice_range.default}
\alias{slice_range.data.frame}
\title{Select a range of records from an object}
\usage{
slice_range(x, range, begin = range[1], end = range[2], ...)

\method{slice_range}{lvec}(x, range, begin = range[1], end = range[2], as_r = FALSE, ...)

\method{slice_range}{ldat}(x, range, begin = range[1], end = range[2], as_r = FALSE, ...)

\method{slice_range}{default}(x, range, begin = range[1], end = range[2], ...)

\method{slice_range}{data.frame}(x, range, begin = range[1], end = range[2], ...)
}
\arguments{
\item{x}{the object to select items from}

\item{range}{a numeric vector with two elements specifying the range to 
select.}

\item{begin}{the first element to select.}

\item{end}{the last element to select.}

\item{...}{ignored; used to pass additional arguments to other methods.}

\item{as_r}{convert the result to an R-object.}
}
\description{
Select a range of records from an object
}
\examples{
x <- as_lvec(1:20)
# Select elements 5:7
slice_range(x, range = c(5, 7))
slice_range(x, begin = 5, end = 7)
slice_range(x, range = c(5, 10), end = 7)
# also works for R-vectors
slice_range(1:20, range = c(5,7))
# convert lvec to rvec
slice_range(x, range = c(5,7), as_r = TRUE)

}
