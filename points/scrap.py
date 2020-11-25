import bs4
import matplotlib.pyplot as plt
import requests
import requests_cache


def exception_handler(request, exception):
    print("Request failed:", request, exception)


def main():
    requests_cache.install_cache('cache_db')
    plt.figure(figsize=(18, 12))
    all_points = []
    all_labels = []
    all_positions = []
    last_edition = 27
    editions = list(range(1, last_edition + 1))
    for edition in editions:
        label = f"IO {edition}"
        all_labels.append(label)
        if edition == 10 or edition == 1:
            continue
        all_positions.append(edition)
        url = f'https://oi.edu.pl/l/{edition}oi_1etap_wyniki/'
        response = requests.get(url)
        soup = bs4.BeautifulSoup(response.content, features='html.parser')
        table = soup.select('.results_table')[0]
        edition_points = []
        for tr in table.find_all('tr')[1:]:
            td = list(list(tr.find_all('td'))[-1].stripped_strings)
            if len(td) == 0:
                continue
            points = int(td[0])
            edition_points.append(points)
        all_points.append(edition_points)
    plt.violinplot(all_points, all_positions, showmeans=True)
    plt.xticks(editions, all_labels)
    plt.title('OI points required overs years')
    plt.ylabel("Points")
    plt.xlabel("Edition")
    plt.savefig('points.png')
    plt.savefig('points.pdf')
    plt.show()


if __name__ == '__main__':
    main()
