#for scraping
from urllib.request import urlopen
from bs4 import BeautifulSoup
from FrequencySummarizer import FrequencySummarizer

def get_only_text(url):
	"""
	return the title and the text of the article
	at the specified url
	"""
	page = urlopen(url).read().decode('utf8')
	soup = BeautifulSoup(page, 'lxml')
	text = ' '.join(map(lambda p: p.text, soup.find_all('p')))
	return soup.title.text, text

feed_xml = urlopen('http://feeds.bbci.co.uk/news/rss.xml').read()
feed = BeautifulSoup(feed_xml.decode('utf8'), 'lxml')
to_summarize = list(map(lambda p: p.text, feed.find_all('guid')))

fs = FrequencySummarizer()
for article_url in to_summarize[:10]:
	title, text = get_only_text(article_url)
	print('--------------------------')
	print(title)
	for s in fs.summarize(text, 2): #if encode code, should to it here
		print('*', s) #if compress code, decode here