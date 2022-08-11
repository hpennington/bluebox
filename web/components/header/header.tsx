import Head from 'next/head'
import Image from 'next/image'
import styles from './header.module.css'

export const Header = () => {
  return (
    <div className={styles.container}>
      <h3 className={styles.logo}>Bluebox blog</h3>
    </div>
  )
}