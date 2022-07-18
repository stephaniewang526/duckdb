import duckdb

class TestMotherDuckConnection(object):
    def test_motherduck_connection(self):
        conn = duckdb.connect(':motherduck:', config={'motherduck_username': 'stephwang',
                                                      'motherduck_password': 'qwerty79',
                                                      'motherduck_host': 'https://motherduck.com/api/v1'})
        result = conn.execute('select * from `motherduck.pond-1.s3://steph-bucket-1/yellow_tripdata_2022-01.parquet`'
                              'limit 10')\
            .fetchall()
