package org.gadek.Leroy;

import java.sql.*;

public class MySQLAccess {
	
	private Connection connect = null;
	private Statement statement = null;
	private PreparedStatement preparedStatement = null;
	private ResultSet resultSet = null;
	
	public String readDB() {
		try {
			Class.forName("com.mysql.jdbc.Driver").newInstance();
			connect = DriverManager.getConnection("jdbc:mysql://localhost:3306/Leroy?user=leroy&password=leroy");
			preparedStatement = connect.prepareStatement("select name from Leroy.users");
			resultSet = preparedStatement.executeQuery();
			return "Użytkownicy:\n  " + writeResultSet(resultSet) + "\n";
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			close();
		}
		return "Użytkownicy:\n  (brak)\n";
	}

	private void close() {
		try {
			if(resultSet != null)
				resultSet.close();
			if(statement != null)
				statement.close();
			if(connect != null)
				connect.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

//	private void writeMD(ResultSet resultSet) throws SQLException {
//		System.out.println("Kolumny:");
//		System.out.println("Table: " + resultSet.getMetaData().getTableName(1));
//		for(int i=1; i<= resultSet.getMetaData().getColumnCount(); ++i)
//			System.out.println("Column " + i + " = " + resultSet.getMetaData().getColumnName(i));
//	}

	private String writeResultSet(ResultSet resultSet) throws SQLException {
		StringBuilder out = new StringBuilder();
		while(resultSet.next())
			out.append(resultSet.getString("name") + ", ");
		String tmp = out.toString();
		return tmp.substring(0, tmp.length()-1);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		MySQLAccess blah = new MySQLAccess();
		blah.readDB();
	}

	public void clearDB() {
		try {
			Class.forName("com.mysql.jdbc.Driver").newInstance();
			connect = DriverManager.getConnection("jdbc:mysql://localhost:3306/Leroy?user=leroy&password=leroy");
			preparedStatement = connect.prepareStatement("delete from Leroy.users");
			preparedStatement.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			close();
		}
	}

	public MySQLAccess() {
		try {
			Class.forName("com.mysql.jdbc.Driver").newInstance();
			connect = DriverManager.getConnection("jdbc:mysql://localhost:3306/Leroy?user=leroy&password=leroy");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void registerDB(String nick) {
		try {
			Class.forName("com.mysql.jdbc.Driver").newInstance();
			connect = DriverManager.getConnection("jdbc:mysql://localhost:3306/Leroy?user=leroy&password=leroy");
			preparedStatement = connect.prepareStatement("insert into Leroy.users(name) values (?)");
			preparedStatement.setString(1, nick);
			preparedStatement.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			close();
		}
	}

	public void updateDB(String nick, String newNick) {
		try {
			Class.forName("com.mysql.jdbc.Driver").newInstance();
			connect = DriverManager.getConnection("jdbc:mysql://localhost:3306/Leroy?user=leroy&password=leroy");
			preparedStatement = connect.prepareStatement("update Leroy.users set name = ? where name = ?");
			preparedStatement.setString(1, newNick);
			preparedStatement.setString(2, nick);
			preparedStatement.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			close();
		}
	}

}
