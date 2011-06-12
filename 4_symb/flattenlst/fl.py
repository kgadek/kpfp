#!/usr/bin/env python
# -*- coding: utf-8

def listp(elem):
    """ Stwierdza, czy podany element jest listą. """
    try: list(elem)
    except: return False
    return True

def flatten(lst):
    """ Spłaszcza listę. """
    res = []
    for i in lst:
        if listp(i): res.extend(flatten(i))
        else: res.append(i)
    return res