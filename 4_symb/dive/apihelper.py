def info(object, spacing=10, collapse=1):
    """Print methods and doc strings.

    Take module, class, list, dictionary or string."""
    methodList = [meth for meth in dir(object) if callable(getattr(object,meth))]
    processFunc = collapse and (lambda s: " ".join(s.split())) or (lambda s: s)
    print "\n".join(["%s %s" % (meth.ljust(spacing),
                                processFunc(str(getattr(object, meth).__doc__)))
                     for meth in methodList])

if __name__ == "__main__":
    print info.__doc__
