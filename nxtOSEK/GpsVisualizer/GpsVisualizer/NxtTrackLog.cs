using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.VisualBasic.FileIO;
using System.IO;
using System.Collections;
using System.Diagnostics;
using System.Windows.Forms;

namespace GpsVisualizer
{

	class NxtTrackLog
	{
		//ログファイルのコードページID
		private const int LOG_CODEPAGE_ID = 932; //cp932 ≒ Shift-JIS
		//csvのセパレータ
		private const string SEPARATOR = ","; //カンマ
		//ヘッダ
        private string[] splitedHeader =
            "Time,Data1,Data2,Battery,Motor Rev A,Motor Rev B,Motor Rev C,ADC S1,ADC S2,ADC S3,ADC S4,I2C".Split(',');

		//読み込んまれて時刻順にソートされたXYAngle
		private List<XYAngle> robotLog;

		//ログのレコード数
		public int Length {
			get {
				return robotLog.Count;
			}
		}

		/**
		 * コンストラクタ
		 */
		public NxtTrackLog(string path)
		{
			//X,Y,Angle以外も含む全CSVログ
			List<int[]> log;

			//CSVファイルを読み込む
			FileStream inFile = new FileStream(path, FileMode.Open);
			StreamReader reader = new StreamReader(inFile,Encoding.GetEncoding(LOG_CODEPAGE_ID));
			//例外発生時はcatchせずそのまま呼び出し元に送信する
			try
			{
				//読み込んでint[]のListにする
				log = ReadLog(reader);
			}
			finally {
				reader.Close();
			}

			//X,Y,Angleのカラムインデックスを取得
			ColumnAssigner assigner = new ColumnAssigner(splitedHeader);
			int xCol = 0;
			int yCol = 0;
			int angleCol = 0;
			try
			{
				if (assigner.ShowDialog() != DialogResult.OK)
				{
					throw new UserCanceledException("ユーザーにより値の割り当てがキャンセルされました。");
				};

				xCol = assigner.xColumnIndex;
				yCol = assigner.yColumnIndex;
				angleCol = assigner.angleColumnIndex;
			}
			finally 
			{
				assigner.Dispose();
			}

			//Time,X,Y,Angleのみを抽出する
			//Timeは無条件に先頭の要素
			robotLog = new List<XYAngle>();
			foreach(int[] record in log){
				robotLog.Add(new XYAngle(
					record[0],
					record[xCol],
					record[yCol],
					record[angleCol]));
			}

			//時刻順にソート
			SortLog(log);
		}

		/**
		 * 指定インデックスに対応したログを返す。
		 */
		public XYAngle getLog(int position) {
			return robotLog[position];
		}

		/**
		 * ログファイルを読み込む。
		 */
		private List<int[]> ReadLog(StreamReader logFile)
        {
            /*
             * 2010/10/21 要望対応
             * ヘッダチェックを緩め、TextFieldParserでの実装に切り替え
            */

            List<int[]> logArray = new List<int[]>();

            using (TextFieldParser csvReader = new TextFieldParser(logFile))
			{
                csvReader.TextFieldType = FieldType.Delimited;
                csvReader.SetDelimiters(SEPARATOR);

                Debug.WriteLine(splitedHeader);

                //ヘッダ確認(カラム数)
                string[] line = csvReader.ReadFields();
                if (line.Length != splitedHeader.Length) //ヘッダ異常(カラム数違い)
                {
                    throw new InvalidNxtLogFileException("ログファイルのヘッダが異常です。");
                }

                //ヘッダ確認(カラム名)
                //0から順番に読み出してくれる保証はないのでEnumeratorは使わない
                for (int i = 0; i < line.Length; i++)
                {
                    if (line[i] != splitedHeader[i]) //ヘッダ異常(カラム名違い)
                    {
                        throw new InvalidNxtLogFileException("ログファイルのヘッダが異常です。");
                    }
                }

                int n = 1;
                //終端に達するまで一行ずつ読み込む
                while (!csvReader.EndOfData)
                {
                    //行数カウント
                    n++;

                    //1行読み込む
                    line = csvReader.ReadFields();

                    //空行だけは読み飛ばす
                    if (line.Length == 0)
                    {
                        continue;
                    }

                    //string[]からint[]へ
                    int[] record = new int[line.Length];
                    for (int i = 0; i < record.Length; i++)
                    {
                        try
                        {
                            record[i] = Int32.Parse(line[i]);
                        }
                        catch (Exception e)
                        {
                            throw new InvalidNxtLogFileException("ログファイルを読み込み中に、次のエラーが発生しました(Line " + n + ")：\n" + e.Message);
                        }
                    }

                    //アレイに追加
                    logArray.Add(record);
                }

			}


            /*
            //最初の一行をチェック
            string line = logFile.ReadLine();
            //
            if (line != header) //ヘッダ異常
            {
                throw new InvalidNxtLogFileException("ログファイルのヘッダが異常です。");
            }

            //終端に達するまで一行ずつ読み込む
            while ((line = logFile.ReadLine()) != null)
            {
                //行数カウント
                n++;

                //空行だけは読み飛ばす
                if (line.Length == 0) {
                    continue;
                }

                //カンマで分割
                string[] recordStr = line.Split(SEPARATOR);
                //規定のカラム数以下なら変なファイル
                if (recordStr.Length != splitedHeader.Length)
                {
                    throw new InvalidNxtLogFileException("ログファイルを読み込み中に、異常なカラム数を検出しました。\n(Line " + n + ")");
                }

                //string[]からint[]へ
                int[] record = new int[recordStr.Length];
                for(int i = 0; i < record.Length; i++){
                    try{
                        record[i] = Int32.Parse(recordStr[i]);
                    }
                    catch(Exception e){
                        throw new InvalidNxtLogFileException("ログファイルを読み込み中に、次のエラーが発生しました(Line " + n + ")：\n" + e.Message);
                    }
                }

                //アレイに追加
                logArray.Add(record);
            }
            */

            return logArray;
		}

		/**
		 * int[]型のレコードを持つListをを1列目の値を元に昇順ソートする。
		 * 各int[]には最低ひとつの要素が格納されている必要がある。
		 * @param list ソート対象のリスト
		 */
		private void SortLog(List<int[]> list) {
			list.Sort(
				delegate(int[] x, int[] y)
				{
					return (x[0] < y[0]) ?
						(-1) :
						(
							(x[0] > y[0])?
							(1)
							:
							(0)
						);
				}
			);
		}

	}
}
