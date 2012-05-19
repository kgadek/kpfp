PHONY: clean py

clean:
	rm -rf __pycache__ parser* *.pyc *~

py: clean
	pypy -c "import p as p" -c "p.main()"

